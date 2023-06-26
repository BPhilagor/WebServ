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

# include <typedefs.hpp>
# include <set>

# include "utils.hpp"
# include "Data.hpp"
# include "Location.hpp"

class Server
{
public:
	Server(const Data &data);
	Server(const Server &other);
	~Server();

	/* property getters */
	const Data &getData() const;
	const std::vector<pairHostPort> &getHostPorts() const;
	const std::set<std::string> &getMethods() const;
	const std::set<std::string> &getServerNames() const;
	const std::map<std::string, Location> &getLocations() const;
	const Location *findLocation(const std::string &path) const;

	/* getters from data */
	const std::string &getDefault(const std::string &prop) const;

	int getBodyLimit() const;
	const std::string &getErrorDir() const;
	const std::string &getUploadDir() const;
	bool getDirListing() const;

	/* property interrogation */
	int isHostPortMatch(const std::string &hostPort) const;
	bool isNameMatch(const std::string &name) const;

private:
	Server();
	Server & operator=(const Server &other);

	const std::string &getPropOrDefaultStr(const std::string &prop) const;
	void _initListen();
	void _initMethod();
	void _initServerNames();
	void _initLocation();
	const Data _data;
	std::vector<pairHostPort> _hostPort;
	std::set<std::string> _methods;
	std::set<std::string> _serverNames;
	std::map<std::string, Location> _locations;
	static const Data _defaultServer;
};

std::ostream &operator<<(std::ostream &os, const Server &s);
std::ostream &operator<<(std::ostream &os, const pairHostPort &o);

#endif /* SERVER_HPP */
