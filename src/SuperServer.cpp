/* ************************************************************************** */
/*                                                     __ __   __             */
/*    WebServ                                         / // / /__ \            */
/*                By: skoulen, bphilago, znichola    / // /_ __/ /            */
/*                                                  /__  __// __/             */
/*                Created: 2023/06/19 15:48:21        /_/  /____/ lausanne.ch */
/*                                                                            */
/* ************************************************************************** */

#include "SuperServer.hpp"
#include <algorithm>

// Default constructor
SuperServer::SuperServer(const Data& data) //TODO : Tester
{
	Data srvs = data.get("server");
	for (int i = 0; i < srvs.count("server"); i++)
	{
		Server newServer = Server(srvs.find("server", i));
		_servers.push_back(newServer);
		FOREACH_VECTOR(pairHostPort, newServer.getHostPorts())
		{
			_ports.insert(it->second);
			_mapHostPort[*it].push_back(newServer);
		}
	}
}

SuperServer::SuperServer(const SuperServer &other) { *this = other; }
SuperServer::~SuperServer() {}
SuperServer &SuperServer::operator=(const SuperServer &other){ (void)other; return *this; }


/* ************************************************************************** */
/* getters                                                                    */
/* ************************************************************************** */

const std::vector<Server> &SuperServer::getServers() const { return _servers; }
const std::set<int> &SuperServer::getPorts() const { return _ports; }

/* ************************************************************************** */
/* propery interrogators                                                      */
/* ************************************************************************** */

const Server *SuperServer::getServerForHostPortAndHostName(const pairHostPort &match, const std::string &hostName) const
{
	mapHostPort::const_iterator foo = _mapHostPort.find(match);
	if (foo != _mapHostPort.end())
		FOREACH_VECTOR(Server, foo->second)
			if (it->isNameMatch(hostName))
				return &(*it);

	pairHostPort zeroMatch = match;
	zeroMatch.first = 0;

	mapHostPort::const_iterator bar = _mapHostPort.find(zeroMatch);
	if (bar != _mapHostPort.end())
		FOREACH_VECTOR(Server, bar->second)
			if (it->isNameMatch(hostName))
				return &(*it);

	if (foo != _mapHostPort.end())
		return &foo->second[0];
	if (bar != _mapHostPort.end())
		return &bar->second[0];
	return NULL;
}


/* ************************************************************************** */
/* stream overloads                                                           */
/* ************************************************************************** */

std::ostream &operator<<(std::ostream &os, const SuperServer &s)
{
	FOREACH_VECTOR(Server, s.getServers())
	{
		os << "\n<server>";
		os << *it << " ";
		os << "</server>";
	}
	// os << "\n   Port = ";
	// FOREACH_VECTOR(int, s.getPorts())
		// os << *it << " ";

	return os;
}
