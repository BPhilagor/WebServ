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

static int genDirListing(const Location &loc, const std::string &path, std::string &body);

int	GET(HTTPResponse &response,
		const Server &server,
		const Location &location,
		const HTTPRequest &request,
		const std::string &path)
{
	int code = 0;
	std::string body = "";

	(void)request;

	switch (location.isMethodAllowed(WS_GET))
	{
	case ws_not_implemented:			code = 501; break; // Not implemented
	case ws_not_allowed:				code = 405; break; // Method not allowed
	case ws_allowed:
		switch (location.getBody(request, path, body))
		{
		case ws_file_not_found:			code = 404; break; // Not found
		case ws_file_no_perm:			code = 403; break; // Forbidden
		case ws_file_found:				code = 200; break; // OK
		case ws_file_isdir:
			std::cout << "Checking for default file: " << path << "\n";
			if (location.isDefaultFileSet())
				switch (location.getBody(request, path + "/" + location.getDefaultFile(), body))
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
		break;
	}
	if (code == 200) // OK
		response.constructReply(code, &body);
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
	int code = 0;
	std::string body = "";

	(void)request;

	switch (location.isMethodAllowed(WS_POST))
	{
	case ws_not_implemented:			code = 501; break; // Not implemented
	case ws_not_allowed:				code = 405; break; // Method not allowed
	case ws_allowed:
		switch (location.getBody(request, path, body))
		{
		case ws_file_not_found:			code = 404; break; // Not found
		case ws_file_no_perm:			code = 403; break; // Forbidden
		case ws_file_found:				code = 200; break; // OK
		case ws_file_isdir:
			std::cout << "Checking for default file: " << path << "\n";
			if (location.isDefaultFileSet())
				switch (location.getBody(request, path + "/" + location.getDefaultFile(), body))
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
		break;
	}
	if (code == 200) // OK
		response.constructReply(code, &body);
	else
		response.constructErrorReply(code, &server);

	return 0;
}

int	DELETE(HTTPResponse &response,
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

static int genDirListing(const Location &loc, const std::string &path, std::string &body)
{
	std::string full_path(loc.getAlias() + path);

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
