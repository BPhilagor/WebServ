/* ************************************************************************** */
/*                                                     __ __   __             */
/*    WebServ                                         / // / /__ \            */
/*                By: skoulen, bphilago, znichola    / // /_ __/ /            */
/*                                                  /__  __// __/             */
/*                Created: 2023/06/19 15:48:21        /_/  /____/ lausanne.ch */
/*                                                                            */
/* ************************************************************************** */

#ifndef SUPERSERVER_HPP
# define SUPERSERVER_HPP
# include "typedefs.hpp"
# include "Data.hpp"
# include "Server.hpp"

class SuperServer
{
private:
	std::vector<Server>	servers;
	std::set<int>		opened_ports;
	mapIpPort			map_IpPort;
public:
// Default constructor
	SuperServer(const Data& data);

// Copy constructor
	SuperServer(const SuperServer &other);

// Destructor
	~SuperServer();

// Copy assignment operator
	SuperServer & operator=(const SuperServer &other);

	const std::vector<Server> &getServers() const;

};

std::ostream &operator<<(std::ostream &os, const SuperServer &s);

#endif /* SUPERSERVER_HPP */
