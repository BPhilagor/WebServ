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
	: _data(other._data), _ipPort(other._ipPort), _port(other._port) {}

Server::~Server() { }
Server &Server::operator=(const Server &other)
{
	(void)other;
	std::cerr << "Can't copy servers, they are immutable\n";
	return *this;
}

const Data &Server::getData() const { return _data; }
const std::vector<pairIpPort> &Server::getIpPorts() const { return _ipPort; }
const std::vector<int> &Server::getPorts() const { return _port; }

/*
	returns the level of match found for the ip/port
	0 no match;
	1 port match
	2 ip and port match
*/
int Server::isIpPortMatch(const std::string &ipPort) const
{
	pairIpPort ipp = utils::getIpPort(ipPort);

	/* this is just duplicated from the init code, something smells */
	int n;
	std::stringstream ss(ipp.second);
	ss >> n;
	if (ss.fail() && (n < 1/*min*/ || n > 10000/*max*/))
	{
		std::cerr << "port can't be a non lkjlk digit\n";
		n = 8080;
	}

	for(std::vector<pairIpPort>::const_iterator it = _ipPort.begin(); it != _ipPort.end(); ++it)
		if (it->first == ipp.first && it->second == ipp.second)
			return 2;
	if (ipp.first == 0)
		for (std::vector<int>::const_iterator it = _port.begin(); it != _port.end(); ++it)
			if (*it == n)
				return 1;
	return 0;
}

void Server::_initListen()
{
	for (int j = 0; j < _data.count("listen"); j++)
	{
		pairIpPort ipPort = utils::getIpPort(_data.find("listen", j).getContent());
		_ipPort.push_back(ipPort);
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

// mapIpPort Server::extractIpPortsMap(const std::vector<Server> &servers)
// {
// 	return mapIpPort();
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
	os << "\nIP/Port =  ";
	FOREACH_VECTOR(pairIpPort, s.getIpPorts())
		os << *it << ", ";

	os << "\n   Port = ";
	FOREACH_VECTOR(int, s.getPorts())
		os << *it << ", ";

	return os;
}


std::ostream &operator<<(std::ostream &os, const pairIpPort &o)
{
	os << o.first << ":" << o.second;
	return os;
}
