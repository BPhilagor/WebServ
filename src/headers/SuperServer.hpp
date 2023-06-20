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
public:
	SuperServer(const Data& data);
	SuperServer(const SuperServer &other);
	~SuperServer();
	SuperServer & operator=(const SuperServer &other);

	const std::vector<Server> &getServers() const;
	const std::set<int> &getPorts() const;

private:
	std::vector<Server>	_servers;
	std::set<int>		_ports;
	mapHostPort			_map_HostPort;

};

std::ostream &operator<<(std::ostream &os, const SuperServer &s);

#endif /* SUPERSERVER_HPP */
