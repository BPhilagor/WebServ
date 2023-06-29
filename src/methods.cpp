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

static int genDirListing(const Location &loc, const std::string &path, std::string &body);
static std::string	deletedFileName(const std::string& path, const std::string& deleted_folder);

int	GET(HTTPResponse &response,
		const Server &server,
		const Location &location,
		const HTTPRequest &request,
		const std::string &path)
{
	int code = 0;
	std::string body = "";
	std::string mime = "";
	bool isBodyCGIgenerated = false;
	(void)request;

	switch (location.getBody(request, path, body, isBodyCGIgenerated, mime))
	{
	case ws_file_not_found:			code = 404; break; // Not found
	case ws_file_no_perm:			code = 403; break; // Forbidden
	case ws_file_found:				code = 200; break; // OK
	case ws_file_isdir:
		if (DP_13 &  DP_MASK)
		std::cout << "Checking for default file: " << path << "\n";
		if (location.isDefaultFileSet())
			switch (location.getBody(request, path + "/" + location.getDefaultFile(),
				body, isBodyCGIgenerated, mime))
			{
			case ws_file_not_found:	code = genDirListing(location, path, body); break; // 404 or 200
			case ws_file_isdir:		code = genDirListing(location, path, body); break; // 404 or 200
			case ws_file_no_perm:	code = 403; break; // Forbidden
			case ws_file_found:		code = 200; break; // OK
			}
		else
			code = genDirListing(location, path, body); break; // 404 or 200
		break;
	}
	if (code == 200)
	{
		if (isBodyCGIgenerated)
		{
			response.parseCGIResponse(body);
		}
		else
		{
			response.constructReply(code, &body, mime);
		}
	}
	else
		response.constructErrorReply(code, &server);
	return 0;
}

int	POST(HTTPResponse &response,
		const Server &server,
		const Location &location,
		const HTTPRequest &request,
		const std::string &path)
{
	(void)response;
	(void)server;
	(void)location;
	(void)request;
	(void)path;
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
		std::cout << "Failed to create deleted files folder '" << deleted_folder <<"': "<<std::strerror(errno) <<std::endl;
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
		response.constructReply(code, &body, "text/html");
	}
	else
	{
		response.constructErrorReply(code, &server);
	}
	return 0;
}

static int genDirListing(const Location &loc, const std::string &path, std::string &body)
{
	std::string full_path(loc.getAlias() + path);

	if (DP_13 &  DP_MASK)
	std::cout << "checking dir listing " << loc.isDirListingSet() << "\n";
	if (loc.isDirListingSet() == false)
		return 404;

	DIR *dir;
	struct dirent *dp;
	if ((dir = opendir(full_path.c_str())) == NULL)
		return 403;

	std::vector<std::string> entry_name;
	while ((dp = readdir(dir)) != NULL)
		entry_name.push_back(std::string(dp->d_name));
	std::string head("<!DOCTYPE html><html><head><title>index of " + path + "</title></head><body><h1>index of " + path + "</h1>\n");
	std::string content("");
	FOREACH_VECTOR(std::string, entry_name)
		content += "<h3><a href=\"/" + path + "/" + *it + "\">" + *it + "</a></h3>\n";

	std::string foot("</body></html>");
	body = head + content + foot;
	closedir(dir);
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
