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

/* use it variable to itterate */
#define FOREACH_VECTOR(type, thing) \
for(std::vector<type>::const_iterator it =thing.begin(); it != thing.end(); ++it)

/* use it variable to itterate */
#define FOREACH_MAP(type, thing) \
for(std::map<type>::const_iterator it =thing.begin(); it != thing.end(); ++it)


Server::Server() : _data(Data()) {}
Server::Server(const Data &data) : _data(data)
{
	_initListen();
}

Server::Server(const Server &other) : _data(other._data) {}
Server::~Server() { }
Server &Server::operator=(const Server &other)
{
	(void)other;
	std::cerr << "Can't copy servers, they are immutable\n";
	return *this;
}

const Data &Server::getData() const { return _data; }
const std::vector<pairIpPort> &Server::getIpPort() const { return _ipPort; }
const std::vector<int> &Server::getPort() const { return _port; }

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
		std::cerr << "port can't be a non digit\n";
		n = 8080;
	}

	for(std::vector<pairIpPort>::const_iterator it = _ipPort.begin(); it != _ipPort.end(); ++it)
		if (it->first == ipp.first && it->second == ipp.second)
			return 2;
	if (ipp.first.empty())
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
		int n;
		std::stringstream ss(ipPort.second);
		ss >> n;
		if (ss.fail() && (n < 1/*min*/ || n > 10000/*max*/))
		{
			std::cerr << "port can't be a non digit\n";
			n = 8080;
		}
		if (ipPort.first.empty())
			_port.push_back(n);
		else
			_ipPort.push_back(ipPort);
	}
}

/* ************************************************************************** */
/* static class functions                                                     */
/* ************************************************************************** */

std::vector<Server> Server::extractServers(const Data &d)
{
	std::vector<Server> servers;

	Data srvs = d.get("server");
	for (int i = 0; i < srvs.count("server"); i++)
		servers.push_back(Server(srvs.find("server", i)));
	return servers;
}

std::set<int> Server::extractPortsSet(const std::vector<Server> &servers)
{
	std::set<int> ports;
	FOREACH_VECTOR(Server, servers)
	{
		std::vector<int> tmp = it->getPort();
		FOREACH_VECTOR(int, tmp)
		{
			ports.insert()
		}
	}
}

mapIpPort Server::extractIpPortsMap(const std::vector<Server> &servers)
{
	return mapIpPort();
}

mapPort Server::extractPortsMap(const std::vector<Server> &servers)
{
	return mapPort();
}

/* ************************************************************************** */
/* stream overloads                                                           */
/* ************************************************************************** */
std::ostream &operator<<(std::ostream &os, const Server &s)
{
	os << "\nIP/Port =  ";
	FOREACH_VECTOR(pairIpPort, s.getIpPort())
		os << *it << " ";

	os << "\n   Port = ";
	FOREACH_VECTOR(int, s.getPort())
		os << *it << " ";

	return os;
}


std::ostream &operator<<(std::ostream &os, const pairIpPort &o)
{
	os << o.first << ":" << o.second;
	return os;
}
