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
	Successful responses: 200 - 299
*/
void HTTPResponse::successfulResponse(const Data &server, int code)
{
	(void)server;
	setReason(reason(code));
}

static const std::string reason(int code)
{
	const static char *reason[9] = {
		"OK",
		"Created",
		"Accepted",
		"Non-Authoritative Information",
		"No Content",
		"Reset Content",
		"Partial Content",
		"Multi-Status",
		"Already Reported"
	};
	code -= 200;
	if (code == 26)
		return "IM Used";
	if (code < 0 || code > 9 )
		return "";
	return reason[code];
}
