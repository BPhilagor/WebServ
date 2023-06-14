/* ************************************************************************** */
/*                                                     __ __   __             */
/*    WebServ                                         / // / /__ \            */
/*                By: skoulen, bphilago, znichola    / // /_ __/ /            */
/*                                                  /__  __// __/             */
/*                Created: 2023/06/14 15:02:52        /_/  /____/ lausanne.ch */
/*                                                                            */
/* ************************************************************************** */

#include "utils.hpp"
#include "handler.hpp"
#include "HTTPResponse.hpp"

std::string handler(const Data &d, const HTTPRequest &req)
{
	(void)d;
	(void)req;

	HTTPResponse res;

	res.setVersion(1, 1);
	res.setCode(200);
	res.setReason("OK");
	res.setHeader("host", "localhost");
	res.setBody("foo bar this that");
	res.setHeader("content-length", "18");

	return res.serialize();
}
