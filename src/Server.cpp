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

const Data Server::_defaultServer = utils::constructDefaultServer();

Server::Server() : _data(Data()) {}
Server::Server(const Data &data) : _data(data)
{
	_initListen();
	_initMethod();
}

Server::Server(const Server &other)
	: _data(other._data), _hostPort(other._hostPort), _methods(other._methods){}

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
const std::vector<std::string> &Server::getMethods() const { return _methods; }

const std::string &Server::getDefault(const std::string &prop) const
{
	if (_defaultServer.count(prop) == 0)
	{
		std::cerr << "default property \"" << prop << "\" not set\n";
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

const std::string &Server::getServerName() const
{
	return getPropOrDefaultStr("server_name");
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
	for (int j = 0; j < _data.count("listen"); j++)
	{
		pairHostPort hostPort = utils::getHostPort(_data.find("listen", j).getContent());
		_hostPort.push_back(hostPort);
	}
}


void Server::_initMethod()
{
	std::string methods = getPropOrDefaultStr("methods");

	// std::cout << "methods : " << methods << "\n";

	while(!methods.empty())
	{
		std::string s1;
		std::string s2;
		utils::split_around_first_c(' ', methods, s1, s2);
		trim_outside_whitespace(s1);
		_methods.push_back(s1);
		methods = s2;
	}
	// std::cout << getMethods()[0] << "asd\n";
}

/* ************************************************************************** */
/* stream overloads                                                           */
/* ************************************************************************** */

std::ostream &operator<<(std::ostream &os, const Server &s)
{
	// std::cout << "srver data is: " <<  s.getData() << "\n";

	os << "\nHost/Port =  ";
	FOREACH_VECTOR(pairHostPort, s.getHostPorts())
	{
		os << *it;
		if (it != s.getHostPorts().end() - 1)
			os << ", ";
	}

	// std::vector<std::string> foo = s.getMethods();
	// std::cout  << "\n methods size: " << foo.size() << " \n";

	// std::cout  << "\n methods size: " << s.getMethods().size() << " \n";

	os << "\n  Methods =  ";
	FOREACH_VECTOR(std::string, s.getMethods())
	{
		os << *it;
		if (it != s.getMethods().end() - 1)
			os << ", ";
	}
	os << "\n";
	return os;
}

std::ostream &operator<<(std::ostream &os, const pairHostPort &o)
{
	os << o.first << ":" << o.second;
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
