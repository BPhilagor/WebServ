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

void getMethod(HTTPResponse &res, const HTTPRequest &req, const Server& server)
{
	// res.setDate();
	// res.setHeader("Host Name", )
	res.constructReply(server, "", 200);
	(void)req;
	// std::cout << "uri is : " << req.getURI() << "\n";
	// std::cout << req << "\n";
}

void postMethod(HTTPResponse &res, const HTTPRequest &req, const Server& server)
{
	(void)res;
	(void)req;
	(void)server;
}

void deleteMethod(HTTPResponse &res, const HTTPRequest &req, const Server& server)
{
	(void)res;
	(void)req;
	(void)server;
}

std::string requestWorker(const Server &server, const HTTPRequest &request, HTTPResponse& response)
{
	// Identify request
	const std::string& method = request.getMethod();

	if (method == "GET")
	{
		getMethod(response, request, server);
	}
	else if (method == "POST")
	{
		postMethod(response, request, server);
	}
	else if (method == "DELETE")
	{
		deleteMethod(response, request, server);
	}
	else
	{
		response.constructErrorReply(501);
	}

	// Construct HTTPResponse
	return response.serialize();
}
