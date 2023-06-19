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

void trim_outside_whitespace(std::string &line)
{
	size_t start = line.find_first_not_of("\t\n\v\f\r ");
	size_t end = line.find_last_not_of("\t\n\v\f\r ");
	if (start == std::string::npos || end == std::string::npos)
		return ;
	line = line.substr(start, end - start + 1);
}

/*
	from stackoverflow: https://stackoverflow.com/questions/216823/how-to-trim-an-stdstring
	trim right and left whitespace from string
*/

static std::string &rtrim(std::string &s, const char *t = LINEAR_WHITESPACE);
static std::string &ltrim(std::string &s, const char *t = LINEAR_WHITESPACE);

std::string&	utils::trim(std::string &s, const char *t)
{
	return (ltrim(rtrim(s, t), t));
}

static std::string&	ltrim(std::string &s, const char *t)
{
	s.erase(0, s.find_first_not_of(t));
	return s;
}

static std::string&	rtrim(std::string &s, const char *t)
{
	s.erase(s.find_last_not_of(t) + 1);
	return s;
}

/*
	remove ending cr if present. replace bare cr with sp

	This is to comply with the http specification.
	It says we must either reject bare CRs or replace them with SP.
	The specification also says we can use LF as line delimiter and ignore the CR that comes before.
*/
std::string&	utils::sanitizeline(std::string& s)
{
	if (s.length() > 0 && s[s.length() - 1] == '\r')
		s.resize(s.length() - 1);
	while (size_t pos = s.find("\r") != std::string::npos)
		s.replace(pos, 1, " ");
	return (s);
}

pairHostPort utils::getHostPort(const std::string &str)
{
	size_t tmp = str.find(':');

	std::stringstream sHost(str.substr(0, tmp));
	std::stringstream sPort(str.substr(tmp + 1, str.size() - tmp));

	int host, port;

	sHost >> host;
	sPort >> port;

	if (sHost.fail())
		host = 0;
	if (sPort.fail())
		port = 8080;

	return pairHostPort(host, port);
}
