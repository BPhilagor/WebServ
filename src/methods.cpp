/* ************************************************************************** */
/*                                                     __ __   __             */
/*    WebServ                                         / // / /__ \            */
/*                By: skoulen, bphilago, znichola    / // /_ __/ /            */
/*                                                  /__  __// __/             */
/*                Created: 2023/06/26 11:39:13        /_/  /____/ lausanne.ch */
/*                                                                            */
/* ************************************************************************** */

#include "utils.hpp"
#include "Location.hpp"
#include "HTTPResponse.hpp"
#include "methods.hpp"

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
	case ws_not_implemented:			code = 500; break;
	case ws_not_allowed:				code = 405; break;
	case ws_allowed:
		switch (location.getBody(path, body))
		{
		case ws_file_not_found:			code = 404; break;
		case ws_no_permission:			code = 402; break; // not sure if this is correct
		case ws_file_found:				code = 200; break; // OK
		case ws_file_isdir:
			std::cout << "Checking for default file\n";
			if (location.isDefaultFileSet())
				switch (location.getBody(path + location.getDefaultFile(), body))
				{
				case ws_file_not_found:	code = 404; break;
				case ws_file_isdir:		code = 404; break;
				case ws_no_permission:	code = 402; break;
				case ws_file_found:		code = 200; break; // OK
				}
			else
				/* file not found */	code = 404;
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
	(void)response;
	(void)server;
	(void)location;
	(void)request;
	(void)path;
	return 0;
}
