/* ************************************************************************** */
/*                                                     __ __   __             */
/*    WebServ                                         / // / /__ \            */
/*                By: skoulen, bphilago, znichola    / // /_ __/ /            */
/*                                                  /__  __// __/             */
/*                Created: 2023/06/14 14:15:21        /_/  /____/ lausanne.ch */
/*                                                                            */
/* ************************************************************************** */

#include "utils.hpp"
#include "HTTPResponse.hpp"
#include "handler.hpp"
#include "requestWorker.hpp"
#include "Server.hpp"

void methodHandlerGET(HTTPResponse &res, const HTTPRequest &req, const Server& server)
{
	res.constructReply(server, "", 200);
	(void)req;
}

void methodHandlerPOST(HTTPResponse &res, const HTTPRequest &req, const Server& server)
{
	(void)res;
	(void)req;
	(void)server;
}

void methodHandlerDELETE(HTTPResponse &res, const HTTPRequest &req, const Server& server)
{
	(void)res;
	(void)req;
	(void)server;
}

void	requestWorker(const Server &server, const HTTPRequest &request, HTTPResponse& response)
{
	// Identify request
	const std::string& method = request.getMethod();

	if (method == "GET")
	{
		methodHandlerGET(response, request, server);
	}
	else if (method == "POST")
	{
		methodHandlerPOST(response, request, server);
	}
	else if (method == "DELETE")
	{
		methodHandlerDELETE(response, request, server);
	}
	else
	{
		response.constructErrorReply(501);
	}
}
