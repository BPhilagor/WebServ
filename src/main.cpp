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

  #ifdef WSL_DISTRO_NAME
		launchServersWSL(servers);
	#else
		launchServersMacOS(servers);
	#endif
	return 0;
}
