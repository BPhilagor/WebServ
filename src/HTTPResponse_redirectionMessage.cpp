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
	Redirection messages: 300 - 399
*/
void HTTPResponse::redirectionMessage(const Data &server, int code)
{
	(void)server;
	setReason(reason(code));
}

static const std::string reason(int code)
{
	const static char *reason[9] = {
		"Multiple Choices",
		"Moved Permanently",
		"Found",
		"See Other",
		"Not Modified",
		"Use Proxy Deprecated",
		"unused",
		"Temporary Redirect",
		"Permanent Redirect"
	};
	code -= 300;
	if (code < 0 || code > 9 )
		return "";
	return reason[code];
}
