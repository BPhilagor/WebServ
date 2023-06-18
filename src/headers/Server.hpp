/* ************************************************************************** */
/*                                                     __ __   __             */
/*    WebServ                                         / // / /__ \            */
/*                By: skoulen, bphilago, znichola    / // /_ __/ /            */
/*                                                  /__  __// __/             */
/*                Created: 2023/06/15 12:38:14        /_/  /____/ lausanne.ch */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP

# include "Data.hpp"
# include <typedefs.hpp>

class Server
{
public:
	Server(const Data &data);
	Server(const Server &other);
	~Server();

	// property getter
	const Data &getData() const;
	const std::vector<pairIpPort> &getIpPort() const;
	const std::vector<int> &getPort() const;

	// property interrogation
	int isIpPortMatch(const std::string &ipPort) const;

private:
	Server();
	Server & operator=(const Server &other);

	void _initListen();
	const Data _data;
	std::vector<pairIpPort> _ipPort;
	std::vector<int> _port;
};

std::ostream &operator<<(std::ostream &os, const Server &s);

#endif /* SERVER_HPP */
