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
	(void)request;
	switch (location.isMethodAllowed(WS_GET))
	{
	case ws_not_implemented:
		// res.constructReply(/* get the html page */, 500); /* should really use this */
		response.constructErrorReply(500);
		break;
	case ws_not_allowed:
		// res.constructReply(/* get the html page */, 405);
		response.constructErrorReply(405);
		break;
	case ws_allowed:
		std::string body;
		if (location.getBody(path, body))
			response.constructErrorReply(404, &server);
		response.constructReply(200, &body);
		break;
	}

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
