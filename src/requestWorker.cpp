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

static void methodHandlerGET(const Server &srv, const HTTPRequest &req,
	HTTPResponse& res, const Location& loc)
{
	res.constructReply(200, NULL);
	std::cout << "Get Method detected" << std::endl;
	(void)loc;
	(void)res;
	(void)req;
	(void)srv;
}

static void methodHandlerPOST(const Server &srv, const HTTPRequest &req,
	HTTPResponse& res, const Location& loc)
{
	(void)loc;
	(void)res;
	(void)req;
	(void)srv;
}

static void methodHandlerDELETE(const Server &srv, const HTTPRequest &req,
	HTTPResponse& res, const Location& loc)
{
	(void)loc;
	(void)res;
	(void)req;
	(void)srv;
}

void	requestWorker(const Server &srv, const HTTPRequest &req, HTTPResponse& res)
{
	// Identify req
	std::cout << "Enter Request Worker" << std::endl;
	const Location *loc = srv.findLocation(req.getURI());
	if (!loc)
	{
		res.constructErrorReply(404);
		return ;
	}

	switch (loc->isMethodAllowed(req.getMethod()))
	{
		case ws_not_allowed : res.constructErrorReply(405); return;
		case ws_not_implemented : res.constructErrorReply(501); return;
		default : break;
	}
	switch (req.getMethod())
	{
		case WS_GET : methodHandlerGET(srv, req, res, *loc); break;
		case WS_POST : methodHandlerPOST(srv, req, res, *loc); break;
		case WS_DELETE : methodHandlerDELETE(srv, req, res, *loc); break;
	}
}
