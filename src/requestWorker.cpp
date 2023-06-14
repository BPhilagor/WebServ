/* ************************************************************************** */
/*                                                     __ __   __             */
/*    WebServ                                         / // / /__ \            */
/*                By: skoulen, bphilago, znichola    / // /_ __/ /            */
/*                                                  /__  __// __/             */
/*                Created: 2023/06/14 14:15:21        /_/  /____/ lausanne.ch */
/*                                                                            */
/* ************************************************************************** */

#include "utils.hpp"
#include "handler.hpp"
#include "requestWorker.hpp"

std::string requestWorker(const Data &d, int socketFD, const std::string &rawRequest)
{
	std::string nonConst = rawRequest;
	HTTPRequest req(nonConst);

	(void)d;

	if (req.getHasValidSyntax())
	{
		send(socketFD, "Request has been rescieved, it's processing\n", 45, 0);
		std::string response = handler(d, req);
		std::cout << req << std::endl;
		return response;
	}

	send(socketFD, "Request has Invalid syntax, try again\n", 39, 0);
	std::cout<<"Invalid syntax"<< std::endl;
	return "";

}
