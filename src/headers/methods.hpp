/* ************************************************************************** */
/*                                                     __ __   __             */
/*    WebServ                                         / // / /__ \            */
/*                By: skoulen, bphilago, znichola    / // /_ __/ /            */
/*                                                  /__  __// __/             */
/*                Created: 2023/06/26 13:13:08        /_/  /____/ lausanne.ch */
/*                                                                            */
/* ************************************************************************** */

#ifndef METHODS_HPP
# define METHODS_HPP

# include <string>
# include <cstring>
# include <sys/time.h>
# include <fcntl.h>

class Server;
class Location;
class HTTPResponse;
class HTTPRequest;


int	GET(HTTPResponse &response,
		const Server &server,
		const Location &location,
		const HTTPRequest &request,
		const std::string &path
		);

int	POST(HTTPResponse &response,
		const Server &server,
		const Location &location,
		const HTTPRequest &request,
		const std::string &path
		);

int	DELETE(HTTPResponse &response,
		const Server &server,
		const Location &location,
		const HTTPRequest &request,
		const std::string &path
		);

#endif /* METHODS_HPP */
