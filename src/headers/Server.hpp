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
	const std::vector<pairHostPort> &getHostPorts() const;

	// property interrogation
	int isHostPortMatch(const std::string &hostPort) const;

private:
	Server();
	Server & operator=(const Server &other);

	void _initListen();
	const Data _data;
	std::vector<pairHostPort> _hostPort;
};

std::ostream &operator<<(std::ostream &os, const Server &s);
std::ostream &operator<<(std::ostream &os, const pairHostPort &o);

#endif /* SERVER_HPP */
