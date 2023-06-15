/* ************************************************************************** */
/*                                                     __ __   __             */
/*    WebServ                                         / // / /__ \            */
/*                By: skoulen, bphilago, znichola    / // /_ __/ /            */
/*                                                  /__  __// __/             */
/*                Created: 2023/06/15 12:38:14        /_/  /____/ lausanne.ch */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP

# include "Data.hpp"

typedef std::pair<std::string, std::string> listenObj;

class Server
{
public:
	Server();
	Server(const Data &data);
	Server(const Server &other);
	~Server();
	Server & operator=(const Server &other);

	// property getter
	const Data &getData() const;
	std::vector<listenObj> getListen() const;


	// std::map<std::string, std::string>

private:
	const Data _data;
};

#endif /* SERVER_HPP */
