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

	/* propery getters */
	const std::vector<Server> &getServers() const;
	const std::set<int> &getPorts() const;
	const std::set<int> &getListeningSockets() const;

	/* propery interrogators */
	bool isMatchHostPort(const pairHostPort &match) const;

	const Server *getServerForHostPortAndHostName(const pairHostPort &match, const std::string &hostName) const;

	const Server *getServerForHostPort(const pairHostPort &match) const;

	void addListeningSocket(int fd);

private:
	std::vector<Server>	_servers;
	std::set<int>		_ports;
	mapHostPort			_mapHostPort;
	std::set<int> 		_listeningSockets;
};

std::ostream &operator<<(std::ostream &os, const SuperServer &s);

#endif /* SUPERSERVER_HPP */
