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
#include "launchServers.hpp"
#include "HTTPRequest.hpp"
#include "HTTPResponse.hpp"

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

	Data servers = data.get("server");

	launchServers(servers);

	return 0;
}
