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

/* use it variable to itterate */
#define FOREACH_VECTOR(type, thing) \
for(std::vector<type>::const_iterator it =thing.begin(); it != thing.end(); ++it)

/* use it variable to itterate */
#define FOREACH_MAP(type, thing) \
for(std::map<type>::const_iterator it =thing.begin(); it != thing.end(); ++it)

class Server
{
public:
	Server(const Data &data);
	Server(const Server &other);
	~Server();

	// property getter
	const Data &getData() const;
	const std::vector<pairIpPort> &getIpPorts() const;
	const std::vector<int> &getPorts() const;

	// property interrogation
	int isIpPortMatch(const std::string &ipPort) const;

	// static utility functions
	static std::vector<Server> extractServers(const Data &d);
	static std::set<int> extractPortsSet(const std::vector<Server> &servers);
	static mapIpPort extractIpPortsMap(const std::vector<Server> &servers);

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
