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

static HTTPResponse generateResponse(const Data &d, const HTTPRequest &req);

std::string requestWorker(const Data &d, int socketFD, const std::string &rawRequest)
{
	HTTPRequest req(rawRequest);
	HTTPResponse res;

	(void)d;


	if (req.getHasValidSyntax())
	{
		std::cout << req << std::endl;

		// find the correct server for the request.
		(void)socketFD;

		if (1 /* server exists */)
		{
			//  give the data for only that server to the handler
			res = generateResponse(d, req);
		}
		else
		{
			res.constructReply(d, 12312); // whatever the correct code for this is.
		}
	}
	else
	{
		std::cout<<"Invalid request syntax"<< std::endl;
		res.constructReply(d, 400);
	}

	return res.serialize();

}

static HTTPResponse generateResponse(const Data &d, const HTTPRequest &req)
{
	HTTPResponse res;

	handler(d, req);

	return res;
}
