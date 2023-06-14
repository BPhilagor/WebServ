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
	Client error responses: 400 - 499
*/
void HTTPResponse::clientErrorResponse(const Data &server, int code)
{
	(void)server;
	setReason(reason(code));
}

static const std::string reason(int code)
{
	const static char *reason[32] = {
		"Bad Request",
		"Unauthorized",
		"Payment Required Experimental",
		"Forbidden",
		"Not Found",
		"Method Not Allowed",
		"Not Acceptable",
		"Proxy Authentication Required",
		"Request Timeout",
		"Conflict",
		"Gone",
		"Length Required",
		"Precondition Failed",
		"Payload Too Large",
		"URI Too Long",
		"Unsupported Media Type",
		"Range Not Satisfiable",
		"Expectation Failed",
		"I'm a teapot",
		"",
		"",
		"Misdirected Request",
		"Unprocessable Content",
		"Locked",
		"Failed Dependency",
		"Too Early Experimental",
		"Upgrade Required",
		"Precondition Required",
		"Too Many Requests",
		"",
		"Request Header Fields Too Large"
	};

	code -= 400;
	if (code == 51)
		return " Unavailable For Legal Reasons";
	if (code < 0 || code > 32 )
		return "";
	return reason[code];
}
