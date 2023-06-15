/* ************************************************************************** */
/*                                                     __ __   __             */
/*    WebServ                                         / // / /__ \            */
/*                By: skoulen, bphilago, znichola    / // /_ __/ /            */
/*                                                  /__  __// __/             */
/*                Created: 2023/06/15 12:38:14        /_/  /____/ lausanne.ch */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

Server::Server() : _data(Data()) {}
Server::Server(const Data &data) : _data(data) {}
Server::Server(const Server &other) : _data(other._data) {}
Server::~Server() { }
Server &Server::operator=(const Server &other)
{
	(void)other;
	std::cerr << "Can't copy data\n";
	return *this;
}
