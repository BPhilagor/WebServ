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
		servers.push_back(newServer);
		FOREACH_VECTOR(pairHostPort, newServer.getHostPorts())
		{
			opened_ports.insert(it->second);
			map_HostPort[*it].push_back(newServer);
		}
	}
}

SuperServer::SuperServer(const SuperServer &other) { *this = other; }
SuperServer::~SuperServer() {}
SuperServer &SuperServer::operator=(const SuperServer &other){ (void)other; return *this; }


/* ************************************************************************** */
/* getters                                                                    */
/* ************************************************************************** */

const std::vector<Server> &SuperServer::getServers() const { return servers; }


/* ************************************************************************** */
/* stream overloads                                                           */
/* ************************************************************************** */

std::ostream &operator<<(std::ostream &os, const SuperServer &s)
{
	FOREACH_VECTOR(Server, s.getServers())
	{
		os << "\n<servers>";
		os << *it << " ";
		os << "\n</servers>\n";
		os << it->getData();
		os << "\n";
	}
	// os << "\n   Port = ";
	// FOREACH_VECTOR(int, s.getPorts())
		// os << *it << " ";

	return os;
}
