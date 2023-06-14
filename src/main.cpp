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
	/* examples on how to use the HTTPResponse */
	/*
	HTTPResponse	resp;

	resp.setVersion(1, 1);
	resp.setCode(200);
	resp.setReason("OK");

	std::cout<<resp<<std::endl;

	resp.setCode(400);
	resp.setReason("Bad Request");
	resp.setHeader("host", "hello world");
	resp.setHeader("content-length", "109");
	resp.setBody("<!DOCTYPE html><html><head><title>400 Bad Request</title></head><body><h1>400 Bad Request</h1></body></html>");

	std::cout<<resp<<std::endl;
	*/

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

	HTTPResponse res;

	res.constructReply(data, 301);

	std::cout << "res is <\n" << res << "\n>\n";

	// return 0;

  #ifdef WSL_DISTRO_NAME
		launchServersWSL(servers);
	#else
		launchServersMacOS(servers);
	#endif
	return 0;
}
