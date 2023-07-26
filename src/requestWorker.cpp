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
#include "methods.hpp"

void	requestWorker(const Server &srv, const HTTPRequest &req, HTTPResponse& res)
{
	std::string new_path;
	const Location *loc = srv.findLocation(req.getURI().path, new_path);
	if (!loc)
	{
		res.constructErrorReply(404, &srv);
	}
	else if (loc->isRedirSet())
	{
		res.constructRedirect(loc->getRedir());
	}
	else
	{
		switch (loc->isMethodAllowed(req.getMethod()))
		{
			case ws_not_allowed : res.constructErrorReply(405); break;
			default :
			switch (req.getMethod())
			{
				case WS_GET : GET(res, srv, *loc, req, new_path); break;
				case WS_POST : POST(res, srv, *loc, req, new_path); break;
				case WS_DELETE : DELETE(res, srv, *loc, req, new_path); break;
				default:
					res.constructErrorReply(501, &srv); /* Method not implemented */
			}
		}
	}

	res.finalize();
}
