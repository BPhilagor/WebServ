/* ************************************************************************** */
/*                                                     __ __   __             */
/*    WebServ                                         / // / /__ \            */
/*                By: skoulen, bphilago, znichola    / // /_ __/ /            */
/*                                                  /__  __// __/             */
/*                Created: 2023/06/01 23:16:37        /_/  /____/ lausanne.ch */
/*                                                                            */
/* ************************************************************************** */

#include "utils.hpp"
#include "Data.hpp"
#include "Server.hpp"
#include "launchServers.hpp"
#include "HTTPRequest.hpp"
#include "HTTPResponse.hpp"
#include "SuperServer.hpp"

int main(int argc, char** argv)
{
	Data data;
	if (argc != 2)
	{
		std::cout << "usage: ./webserv path/to/config" << std::endl;
		std::cout << "still loading a server with the default config" << std::endl;
		Data::readFile(data, NULL);
	}
	else
		Data::readFile(data, argv[1]);

	std::cout << "\nconfig:\n";
	Data::print(data, 2);
	std::cout << "\n~~~~~~~~~~~~~~~~~~~~~~~~\n\n";

	SuperServer config(data);

	std::cout << config << "\n";

	pairHostPort foo(utils::addrStringToInt("127.0.0.1"), htons(4242));
	std::cout << "found this server: " << *config.getServerForHostPortAndHostName(foo, "asd") << "\n";

	launchServers(config);

	return 0;
}
