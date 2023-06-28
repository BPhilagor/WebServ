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
	// Identify req
	std::cout << "Enter Request Worker" << std::endl;
	std::string new_path;
	const Location *loc = srv.findLocation(req.getURI().path, new_path);
	if (!loc)
	{
		res.constructErrorReply(404, &srv);
		return ;
	}

	// switch (loc->isMethodAllowed(req.getMethod()))
	// {
	// 	case ws_not_allowed : res.constructErrorReply(405); return;
	// 	case ws_not_implemented : res.constructErrorReply(501); return;
	// 	default : break;
	// }

	std::string path = "/";

	switch (req.getMethod())
	{
		case WS_GET : GET(res, srv, *loc, req, new_path); break;
		case WS_POST : POST(res, srv, *loc, req, new_path); break;
		case WS_DELETE : DELETE(res, srv, *loc, req, new_path); break;
		default:
			res.constructErrorReply(501, &srv); /* Method not implemented */
	}
}
