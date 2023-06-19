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

static HTTPResponse getMethode()
{
	HTTPResponse result;

	return result;
}

static HTTPResponse postMethode()
{
	HTTPResponse result;

	return result;
}

static HTTPResponse deleteMethode()
{
	HTTPResponse result;

	return result;
}

std::string resolveMethod(const Server& server, const HTTPRequest &request)
{
	HTTPResponse result;


	(void) server;
	// Identify request
	const std::string& methode = request.getMethod();
	if (methode.compare("GET"))
	{
		result = getMethode();
	} else if (methode.compare("POST"))
	{
		result = postMethode();
	} else if (methode.compare("DELETE"))
	{
		result = deleteMethode();
	} else
	{
		// Manage Error
		;
	}
	
	// Construct HTTPResponse
	
	return result.serialize();
}

// std::string requestWorker(const SuperServeur& superServer, const HTTPRequest &request)
// {
// 	// Check HTTP Request
// 	// Find Server
// 	const Server& server = superServer.find(/*...*/);
// 	resolveMethod(server, request);
// }
