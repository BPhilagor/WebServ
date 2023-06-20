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

void getMethod(HTTPResponse &res, const Server& server)
{
	// res.setDate();
	// res.setHeader("Host Name", )
	(void)server;
	res.constructReply(server, "", 200);
	std::cout << "bar {" <<  res.serialize() << "} foo \n";
}

void postMethod(HTTPResponse &res, const Server& server)
{
	(void)res;
	(void)server;
}

void deleteMethod(HTTPResponse &res, const Server& server)
{
	(void)res;
	(void)server;
}

std::string requestWorker(const Server &server, const HTTPRequest &request)
{
	HTTPResponse res;

	// Identify request
	const std::string& method = request.getMethod();

	std::cout << method << "skdlfj\n";

	if (method == "GET")
	{
		getMethod(res, server);
	}
	else if (method == "POST")
	{
		postMethod(res, server);
	}
	else if (method == "DELETE")
	{
		deleteMethod(res, server);
	}
	else
	{
		// Manage Error
		;
	}

	// Construct HTTPResponse
	return res.serialize();
}
