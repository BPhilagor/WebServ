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
	Server error responses: 500 - 599
*/
void HTTPResponse::serverErrorResponse(const Data &server, int code)
{
	(void)server;
	setReason(reason(code));
}

static const std::string reason(int code)
{
	const static char *reason[12] = {
		"Internal Server Error",
		"Not Implemented",
		"Bad Gateway",
		"Service Unavailable",
		"Gateway Timeout",
		"HTTP Version Not Supported",
		"Variant Also Negotiates",
		"Insufficient Storage",
		"Loop Detected",
		"Not Extended",
		"Network Authentication Required"
	};
	code -= 500;
	if (code < 0 || code > 12 )
		return "";
	return reason[code];
}
