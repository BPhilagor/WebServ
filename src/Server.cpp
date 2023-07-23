/* ************************************************************************** */
/*                                                     __ __   __             */
/*    WebServ                                         / // / /__ \            */
/*                By: skoulen, bphilago, znichola    / // /_ __/ /            */
/*                                                  /__  __// __/             */
/*                Created: 2023/06/15 12:38:14        /_/  /____/ lausanne.ch */
/*                                                                            */
/* ************************************************************************** */

#include <cstdlib>

#include "Server.hpp"
#include "utils.hpp"
#include "debugDefs.hpp"

#define DEFAULT_LISTEN "0:8080"

const Data Server::_defaultServer = utils::constructDefaultServer();

Server::Server() : _data(Data()) {}
Server::Server(const Data &data) : _data(data)
{
	_initListen();
	_initMethod();
	_initServerNames();
	_initLocation();
}

Server::Server(const Server &other) :
	_data(other._data),
	_hostPort(other._hostPort),
	_methods(other._methods),
	_serverNames(other._serverNames),
	_locations(other._locations)
	{}

Server::~Server() { }

Server &Server::operator=(const Server &other)
{
	(void)other;
	std::cerr << "Can't copy servers, they are immutable\n";
	return *this;
}

/* ************************************************************************** */
/* getters                                                                    */
/* ************************************************************************** */

const Data &Server::getData() const { return _data; }
const std::vector<pairHostPort> &Server::getHostPorts() const { return _hostPort; }
const std::set<std::string> &Server::getMethods() const { return _methods; }
const std::set<std::string> &Server::getServerNames() const { return _serverNames; }

const std::map<std::string, Location> &Server::getLocations() const
{
	return _locations;
}


const std::string &Server::getDefault(const std::string &prop) const
{
	if (_defaultServer.count(prop) == 0)
	{
		std::cerr << ESC_COLOR_RED << "default property \"" << prop << "\" not set"
			<< ESC_COLOR_RESET << std::endl;
		exit(42);
	}
	return _defaultServer.find(prop).getContentRef();
}

int Server::getBodyLimit() const
{
	if (_data.count("body_limit") == 0)
		return utils::toInt(getDefault("body_limit"));
	else
		return utils::toInt(_data.find("body_limit").getContentRef());
}

const std::string &Server::getErrorDir() const
{
	return getPropOrDefaultStr("error_dir");
}

const std::string &Server::getUploadDir() const
{
	return getPropOrDefaultStr("upload_dir");
}

bool Server::getDirListing() const
{
	if (_data.count("upload_dir") == 0)
		return getDefault("upload_dir") == "true" ? true : false;
	else
		return _data.find(
			"upload_dir").getContentRef() == "true" ? true : false;
}

const Location *Server::findLocation(const std::string &path, std::string &new_path) const
{
	std::string tmp_path = path;

	while (1)
	{
		if (DP_4 & DP_MASK)
		std::cout << tmp_path << std::endl;
		const std::map<std::string, Location>::const_iterator it = _locations.find(tmp_path);
		if (it != _locations.end())
		{
			if (DP_4 & DP_MASK)
			std::cout << "Location found !!" << std::endl;
			new_path = path.substr(tmp_path.length());
			if (DP_4 & DP_MASK)
			std::cout << "New path = " << new_path << std::endl;
			return &it->second;
		}
		if (tmp_path.length() == 1)
			break ;
		size_t index = tmp_path.find_last_of('/', tmp_path.length());
		if (index == std::string::npos)
			break ;
		if (index != tmp_path.length() - 1)
			tmp_path = tmp_path.substr(0, index + 1);
		else
			tmp_path = tmp_path.substr(0, index);
	}
	if (DP_4 & DP_MASK)
	std::cout << "Location not found !!" << std::endl;
	return NULL;
}

/* ************************************************************************** */
/* property interrogators                                                     */
/* ************************************************************************** */

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
	// int n;
	// std::stringstream ss(ipp.second);
	// ss >> n;
	// if (ss.fail() && (n < 1/*min*/ || n > 10000/*max*/))
	// {
	// 	std::cerr << "port can't be a non lkjlk digit\n";
	// 	n = 8080;
	// }

	for(std::vector<pairHostPort>::const_iterator it = _hostPort.begin(); it != _hostPort.end(); ++it)
		if (it->first == ipp.first && it->second == ipp.second)
			return 2;
	// if (ipp.first == 0)
	// 	for (std::vector<int>::const_iterator it = _port.begin(); it != _port.end(); ++it)
	// 		if (*it == n)
	// 			return 1;
	return 0;
}

bool Server::isNameMatch(const std::string &name) const
{
	return _serverNames.count(name);
}

const std::string &Server::getPropOrDefaultStr(const std::string &prop) const
{
	if (_data.count(prop) == 0)
		return getDefault(prop);
	else
		return _data.find(prop).getContentRef();
}

/* ************************************************************************** */
/* initialisation helper functions                                            */
/* ************************************************************************** */

void Server::_initListen()
{
	int nbr = _data.count("listen");
	if (nbr == 0)
	{
		pairHostPort hostPort = utils::getHostPort(DEFAULT_LISTEN);
		_hostPort.push_back(hostPort);
		return ;
	}
	for (int j = 0; j < nbr; j++)
	{
		pairHostPort hostPort = utils::getHostPort(_data.find("listen", j).getContent());
		_hostPort.push_back(hostPort);
	}
}

void Server::_initMethod()
{
	std::string str = getPropOrDefaultStr("methods");

	// std::cout << "methods : " << methods << "\n";

	while(!str.empty())
	{
		std::string s1;
		std::string s2;
		utils::split_around_first_c(' ', str, s1, s2);
		trim_outside_whitespace(s1);
		_methods.insert(s1);
		str = s2;
	}
	// std::cout << getMethods()[0] << "asd\n";
}

void Server::_initServerNames()
{
	std::string str = getPropOrDefaultStr("server_name");

	while(!str.empty())
	{
		std::string s1;
		std::string s2;
		utils::split_around_first_c(' ', str, s1, s2);
		trim_outside_whitespace(s1);
		_serverNames.insert(s1);
		str = s2;
	}
}

void Server::_initLocation()
{
	int x = 0;
	if ( (x = _data.count("location")) == 0)
	{
		std::cerr << "Invalid server: no location set" << std::endl;
		return ;
	}
	for (int i = 0; i < x; i++)
		_locations[_data.find("location", i).getContent()]
			= Location(_data.find("location", i));
}

/* ************************************************************************** */
/* stream overloads                                                           */
/* ************************************************************************** */

std::ostream &operator<<(std::ostream &os, const Server &s)
{
	// std::cout << "srver data is: " <<  s.getData() << "\n";

	os << "\n     Host/Port =  ";
	FOREACH_VECTOR(pairHostPort, s.getHostPorts())
	{
		os << *it;
		if (it != s.getHostPorts().end() - 1)
			os << ", ";
	}

	// std::vector<std::string> foo = s.getMethods();
	// std::cout  << "\n methods size: " << foo.size() << " \n";

	// std::cout  << "\n methods size: " << s.getMethods().size() << " \n";

	os << "\n       Methods =  ";
	FOREACH_SET(std::string, s.getMethods())
	{
		os << *it;
		if (it != s.getMethods().end())
			os << " ";
	}
	os << "\n  Server Names =  ";
	FOREACH_SET(std::string, s.getServerNames())
	{
		os << *it;
		if (it != s.getServerNames().end())
			os << " ";
	}

	os << "\n      Locations = ";
	FOREACH_MAP(Location, s.getLocations())
	{
		os << it->first;
		os << it->second;
	}
	os << "\n";
	return os;
}

std::ostream &operator<<(std::ostream &os, const pairHostPort &o)
{
	os << utils::addrIntToString(o.first) << ":" << ntohs(o.second);
	return os;
}

// std::ostream &operator<<(std::ostream &os, const std::vector<std::string> &o)
// {
// 	FOREACH_VECTOR(std::string, o)
// 	{
// 		os << *it;
// 		if (it != o.end())
// 			os << ", ";
// 	}
// 	return os;
// }
