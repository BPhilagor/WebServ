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

#define BACK_LOG 10

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

	// launchServers(servers);

	// return 0;

	// Data::print(servers, 2);
	// std::cout << servers << "\n";

	(void) argc;
	(void) argv;

	// Creating the socket
	int server_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (server_fd < 0) {
		std::cout << "Error when creating socket" << std::endl;
		return 1;
	}

	// Creating the sockaddr_in struct
	struct sockaddr_in address;
	const int addrlen = sizeof(address);
	const int PORT = servers.find("server").find("listen").getInt();
	memset((char *)&address, 0, sizeof(address));
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = htonl(INADDR_ANY);
	address.sin_port = htons(PORT);

	//Binding the socket with the wanted port
	if (bind(server_fd, (struct sockaddr *)&address, addrlen)) {
		std::cout << "Error when binding socket" << std::endl;
		return 1;
	}

	// Start listening
	if (listen(server_fd, BACK_LOG) < 0) {
		std::cout << "Error when listening" << std::endl;
		return 1;
	}
	while (1) {
		std::cout << "Waiting for new connection" << std::endl;

		//Reading the message
		char buffer[1024];
		int new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen);
		if (new_socket < 0) {
			std::cout << "Error when accepting request" << std::endl;
			return 1;
		}
		std::cout << "Connection established" << std::endl;
		int read_size;
		do {
			read_size = read(new_socket, buffer, 1024);
			std::cout << buffer;
		} while (read_size == 1024);
		//Sending a message
		std::string message = "Hello c'est webServ";
		send_file_to_socket("html/index.html", new_socket);
		close(new_socket);
	}
	return 0;
}
