/* ************************************************************************** */
/*                                                     __ __   __             */
/*    WebServ                                         / // / /__ \            */
/*                By: skoulen, bphilago, znichola    / // /_ __/ /            */
/*                                                  /__  __// __/             */
/*                Created: 2023/06/01 23:16:37        /_/  /____/ lausanne.ch */
/*                                                                            */
/* ************************************************************************** */

#include "utils.hpp"

int send_to_socket(const std::string &message, int socket_fd) {
	return write(socket_fd, message.c_str(), message.length());
}

int send_file_to_socket(const std::string &filename, int socket_fd) {
	std::ifstream file;
	file.open(filename.c_str());

	std::string line;
	std::string message;
	while (std::getline(file, line)) {
		message += line + "\r\n";
	}
	return send_to_socket(message, socket_fd);
}
