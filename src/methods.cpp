/* ************************************************************************** */
/*                                                     __ __   __             */
/*    WebServ                                         / // / /__ \            */
/*                By: skoulen, bphilago, znichola    / // /_ __/ /            */
/*                                                  /__  __// __/             */
/*                Created: 2023/06/26 11:39:13        /_/  /____/ lausanne.ch */
/*                                                                            */
/* ************************************************************************** */

#include <sys/types.h>
#include <dirent.h>
#include <libgen.h>

#include "utils.hpp"
#include "Location.hpp"
#include "HTTPResponse.hpp"
#include "methods.hpp"
#include "debugDefs.hpp"

static int genDirListing(const Location &loc, const std::string &path, HTTPResponse& response);
static std::string	deletedFileName(const std::string& path, const std::string& deleted_folder);

/* return ENOENT, EPERM, EISDIR or 0*/
/* Maybe this should be able to set 500 Internal server error, for example if ENOMEM (no more kernel memory) */
int check_file(const char *path)
{
	if (access(path, R_OK) != 0)
	{
		std::cout << std::strerror(errno) <<std::endl;
		if (errno == ENOENT || errno == ENOTDIR)
			return (ENOENT);
		else
			return (EPERM);
	}
	else
	{
		/* check if file is directory*/
		struct stat path_stat;
		stat(path, &path_stat);
		if (S_ISDIR(path_stat.st_mode))
			return (EISDIR);
		else
			return (0);
	}
}

void	getOrPost(HTTPResponse &response, const Server &server, const Location& location, const HTTPRequest& request, const std::string& path)
{
	std::string real_path = location.getRealPath(path);
	int file_status = check_file(real_path.c_str());

	if (file_status == ENOENT)
		response.setCode(404);
	else if (file_status == EPERM)
		response.setCode(403);
	else if (file_status == EISDIR)
	{
		if (DP_13 & DP_MASK)
			std::cout << "URI is a directory. Checking for default file: " << path << std::endl;

		if (location.isDefaultFileSet())
		{
			std::string index_path = path + "/" + location.getDefaultFile();
			std::string real_index_path = location.getRealPath(index_path);
			int index_file_status = check_file(real_index_path.c_str());
			if (index_file_status != 0)
			{
				goto generate_dir_listing;
			}
			else
			{
				real_path = real_index_path;
				goto file_is_found;
			}
		}
		else
		{
			generate_dir_listing:
			if (!location.isDirListingSet())
				response.setCode(403);
			else
				response.setCode(genDirListing(location, path, response));
		}
	}
	else
	{
		file_is_found:
		if (location.isCGIrequired(real_path))
		{
			if (!response.serveDynamicFile(location, real_path, request))
				response.setCode(403);
		}
		else
			response.serveStaticFile(real_path);
	}

	/* check the status code produced in the operations above. If necessary, serve an error reply */
	if (response.getCode() >= 400) /* errors 4xx and 5xx */
	{
		response.constructErrorReply(response.getCode(), &server);
	}
}

int	GET(HTTPResponse &response,
		const Server &server,
		const Location &location,
		const HTTPRequest &request,
		const std::string &path)
{
	getOrPost(response, server, location, request, path);
	return 0;
}

int	POST(HTTPResponse &response,
		const Server &server,
		const Location &location,
		const HTTPRequest &request,
		const std::string &path)
{
	getOrPost(response, server, location, request, path);
	return 0;
}

int	DELETE(HTTPResponse &response,
		const Server &server,
		const Location &location,
		const HTTPRequest &request,
		const std::string &path)
{
	(void)request;

	int	code;

	/* get real path of location */
	std::string real_path = location.getRealPath(path);

	/* should we configure this? */
	std::string	deleted_folder = "deleted";

	/* make sure this folder is created */
	if (mkdir(deleted_folder.c_str(), 0755) != 0 && errno != EEXIST)
	{
		std::cerr << "Failed to create deleted files folder '" << deleted_folder <<"': "<<std::strerror(errno) <<std::endl;
		code = 500;
	}

	std::string new_name = deletedFileName(path, deleted_folder);

	if (rename(real_path.c_str(), new_name.c_str()) != 0)
	{
		if (errno == ENOENT || errno == ENOTDIR)
			code = 404;
		else if (errno == EPERM || errno == EACCES)
			code = 403;
		else
			code = 500;
	}
	else
	{
		code = 200;
	}

	/* generate the reply */
	if (code == 200)
	{
		/* generate reply for DELETE */
		std::string body = "<!DOCTYPE html><html><head><title>200 OK</title></head><body><h1>200 OK</h1><p>Resource was moved successfully.</body></html>";
		response.setBody(body);
		response.setHeader("content-type", "text/html");
		response.setHeader("content-length", SSTR(body.size()));
		response.setCode(200);
	}
	else
	{
		response.constructErrorReply(code, &server);
	}
	return 0;
}

static int genDirListing(const Location &loc, const std::string &path, HTTPResponse& response)
{
	std::string	real_path = loc.getRealPath(path);
	std::string	path_to_file;

	DIR *dir;
	struct dirent *dp;
	if ((dir = opendir(real_path.c_str())) == NULL)
		return 500;

	std::vector<std::string> entry_name;
	while ((dp = readdir(dir)) != NULL)
		entry_name.push_back(std::string(dp->d_name));

	std::string head("<!DOCTYPE html><html><head><title>index of " + path + "</title></head><body><h1>Index of /" + path + "</h1><ul>");
	std::string content("");
	FOREACH_VECTOR(std::string, entry_name)
	{
		path_to_file = path + ((path != "")?"/":"") + *it;
		content += "<li><a href=\"/" + path_to_file + "\">" + *it + "</a></li>";
	}
	std::string foot("</ul></body></html>");

	closedir(dir);

	response.setBody(head + content + foot);
	response.setHeader("content-length", SSTR(response.getBody().size()));
	response.setHeader("content-type", "text/html");
	response.setCode(200);

	return 200;
}

static std::string	deletedFileName(const std::string& path, const std::string& deleted_folder)
{
	std::string base_name = path.substr(path.find_last_of("/\\") + 1);

	/* get current timestamp in milliseconds */
	struct timeval tp;
	gettimeofday(&tp, NULL);
	long int ms = tp.tv_sec * 1000 + tp.tv_usec / 1000;
	std::string timestamp = SSTR(ms);

	std::string new_name = deleted_folder + "/" + timestamp + "-" + base_name;

	return (new_name);
}
