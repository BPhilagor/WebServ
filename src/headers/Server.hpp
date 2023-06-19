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
# include <set>

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

	// static utility functions
	static std::vector<Server> extractServers(const Data &d);
	static std::set<int> extractPortsSet(const std::vector<Server> &servers);
	static mapIpPort extractIpPortsMap(const std::vector<Server> &servers);
	static mapPort extractPortsMap(const std::vector<Server> &servers);

private:
	Server();
	Server & operator=(const Server &other);

	void _initListen();
	const Data _data;
	std::vector<pairIpPort> _ipPort;
	std::vector<int> _port;
};

std::ostream &operator<<(std::ostream &os, const Server &s);
std::ostream &operator<<(std::ostream &os, const pairIpPort &o);

#endif /* SERVER_HPP */
