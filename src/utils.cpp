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

std::string trim_outside_whitespace(const std::string &line)
{
	size_t start = line.find_first_not_of("\t\n\v\f\r ");
	size_t end = line.find_last_not_of("\t\n\v\f\r ");
	if (start == std::string::npos || end == std::string::npos)
		return line;
	return line.substr(start, end - start + 1);
}
