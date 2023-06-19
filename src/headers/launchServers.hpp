/* ************************************************************************** */
/*                                                     __ __   __             */
/*    WebServ                                         / // / /__ \            */
/*                By: skoulen, bphilago, znichola    / // /_ __/ /            */
/*                                                  /__  __// __/             */
/*                Created: 2023/06/01 23:16:37        /_/  /____/ lausanne.ch */
/*                                                                            */
/* ************************************************************************** */

#ifndef LAUNCHSERVERS_HPP
# define LAUNCHSERVERS_HPP
# define BACK_LOG 10

#ifdef WSL_DISTRO_NAME
	#include <sys/epoll.h>
#else
	#include <sys/event.h>
#endif

# include "utils.hpp"
# include "Data.hpp"
#include <set>
#include <iterator> //for std::ostream_iterator
#include <algorithm> //for std::copy
#include <iostream> //for std::cout
#include <fcntl.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <cstring>

#include "HTTPRequest.hpp"
#include "HTTPParser.hpp"
#include "requestWorker.hpp"


#define BUFFER_SIZE	32
#define MAX_EVENTS	10


#ifdef WSL_DISTRO_NAME
# define SEND_FLAGS MSG_NOSIGNAL
#else
# define SEND_FLAGS 0
#endif


/* launchServers.hpp */

void	launchServers(const Data& config);

/* launchServersUtils.hpp */

void	addSocketToEventQueue(int eqfd, int socket_fd);
void	addPassiveSocketsToQueue(int eqfd, std::set<int> listeningSockets);
void	readHandler(int fd, std::map<int, HTTPParser>& messages);
void	writeHandler(int fd, std::map<int, HTTPParser>& messages);
void	printClientAddress(int fd);
void	findPorts(const Data & servers, std::set<int> &ports, mapIpPort &map_IpPort, mapPort &map_Port);
int		openSockets(const std::set<int>& ports, std::set<int>& sockets);
bool	isListenSocket(int fd, std::set<int>& listenSockets);
void	establishConnection(int ev_fd, std::map<int, HTTPParser> &messages, int eqfd);

#endif /* LAUNCHSERVERS_HPP */
