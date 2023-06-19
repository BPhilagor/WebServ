/* ************************************************************************** */
/*                                                     __ __   __             */
/*    WebServ                                         / // / /__ \            */
/*                By: skoulen, bphilago, znichola    / // /_ __/ /            */
/*                                                  /__  __// __/             */
/*                Created: 2023/06/15 12:38:14        /_/  /____/ lausanne.ch */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include "utils.hpp"
#include <cstdlib>

Server::Server() : _data(Data()) {}
Server::Server(const Data &data) : _data(data)
{
	_initListen();
}

Server::Server(const Server &other) 
	: _data(other._data), _hostPort(other._hostPort) {}

Server::~Server() { }
Server &Server::operator=(const Server &other)
{
	(void)other;
	std::cerr << "Can't copy servers, they are immutable\n";
	return *this;
}

const Data &Server::getData() const { return _data; }
const std::vector<pairHostPort> &Server::getHostPorts() const { return _hostPort; }

/*
	returns the level of match found for the ip/port
	0 no match;
	1 port match
	2 ip and port match
*/
int Server::isHostPortMatch(const std::string &hostPort) const
{
	pairHostPort ipp = utils::getHostPort(hostPort);

	/* this is just duplicated from the init code, something smells */
	int n;
	std::stringstream ss(ipp.second);
	ss >> n;
	if (ss.fail() && (n < 1/*min*/ || n > 10000/*max*/))
	{
		std::cerr << "port can't be a non lkjlk digit\n";
		n = 8080;
	}

	for(std::vector<pairHostPort>::const_iterator it = _hostPort.begin(); it != _hostPort.end(); ++it)
		if (it->first == ipp.first && it->second == ipp.second)
			return 2;
	// if (ipp.first == 0)
	// 	for (std::vector<int>::const_iterator it = _port.begin(); it != _port.end(); ++it)
	// 		if (*it == n)
	// 			return 1;
	return 0;
}

void Server::_initListen()
{
	for (int j = 0; j < _data.count("listen"); j++)
	{
		pairHostPort hostPort = utils::getHostPort(_data.find("listen", j).getContent());
		_hostPort.push_back(hostPort);
	}
}

/* ************************************************************************** */
/* static class functions                                                     */
/* ************************************************************************** */

// std::set<int> Server::extractPortsSet(const std::vector<Server> &servers)
// {
// 	std::set<int> ports;
// 	FOREACH_VECTOR(Server, servers)
// 	{
// 		std::vector<int> tmp = it->getPorts();
// 		FOREACH_VECTOR(int, tmp)
// 		{
// 			ports.insert()
// 		}
// 	}
// }

// mapHostPort Server::extractHostPortsMap(const std::vector<Server> &servers)
// {
// 	return mapHostPort();
// }

// mapPort Server::extractPortsMap(const std::vector<Server> &servers)
// {
// 	return mapPort();
// }

/* ************************************************************************** */
/* stream overloads                                                           */
/* ************************************************************************** */
std::ostream &operator<<(std::ostream &os, const Server &s)
{
	os << "\nHost/Port =  ";
	FOREACH_VECTOR(pairHostPort, s.getHostPorts())
		os << *it << ", ";

	return os;
}


std::ostream &operator<<(std::ostream &os, const pairHostPort &o)
{
	os << o.first << ":" << o.second;
	return os;
}
