/* ************************************************************************** */
/*                                                     __ __   __             */
/*    WebServ                                         / // / /__ \            */
/*                By: skoulen, bphilago, znichola    / // /_ __/ /            */
/*                                                  /__  __// __/             */
/*                Created: 2023/06/14 19:28:33        /_/  /____/ lausanne.ch */
/*                                                                            */
/* ************************************************************************** */

#include "HTTPResponse.hpp"

static const std::string reason(int code);

/*
	Information responses: 100 - 199
*/
void HTTPResponse::informationalResponse(const Data &server, int code)
{
	(void)server;
	setReason(reason(code));
}

static const std::string reason(int code)
{
	const static char *reason[4] = {
		"Continue",
		"Switching Protocols",
		"Processing (WebDAV)",
		"Early Hints Experimental"
	};
	code -= 100;
	if (code < 0 || code > 4 )
		return "";
	return reason[code];
}
