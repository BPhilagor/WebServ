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
#include "debugDefs.hpp"

int main(int argc, char** argv, char **env)
{
	srand((unsigned)time(NULL) * getpid());
	Data data;
	if (argc != 2)
	{
		std::cout << "usage: ./webserv path/to/config" << std::endl;
		std::cout << "still loading a server with the default config" << std::endl;
		Data::readFile(data, NULL);
	}
	else
		Data::readFile(data, argv[1]);

	if (DP_5 & DP_MASK)
	{
		std::cout << "\nconfig:\n";
		Data::print(data, 2);
		std::cout << "\n~~~~~~~~~~~~~~~~~~~~~~~~\n\n";
	}

	SuperServer config(data);

	BufferManager::config = &config;
	if (DP_8 & DP_MASK)
	std::cout << config << "\n";

	openSockets(config.getPorts(), config);

	launchServers(config, argv, env);

	return 0;
}
