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

# define BACK_LOG 10 /* is this used??*/

# ifdef __linux__
#  include <sys/epoll.h>
# else
#  include <sys/event.h>
# endif

# include <set>
# include <iterator>  //for std::ostream_iterator
# include <algorithm> //for std::copy
# include <iostream>  //for std::cout
# include <fcntl.h>
# include <netinet/in.h>
# include <arpa/inet.h>
# include <cstring>

# include "utils.hpp"
# include "Data.hpp"
# include "SuperServer.hpp"
# include "HTTPRequest.hpp"
# include "BufferManager.hpp"
# include "requestWorker.hpp"

# define BUFFER_SIZE	128
# define MAX_EVENTS		10


# ifdef __linux__
#  define SEND_FLAGS MSG_NOSIGNAL
# else
#  define SEND_FLAGS 0
# endif

#define EVENT_FILTER_READ	0
#define EVENT_FILTER_WRITE	1

#define EVENT_ACTION_ADD	2
#define EVENT_ACTION_DELETE	3

/* launchServers.hpp */

void	launchServers(const SuperServer &config);

/* launchServersUtils.hpp */

void	addSocketToEventQueue(int eqfd, int socket_fd);
void	addPassiveSocketsToQueue(int eqfd, std::set<int> listeningSockets);
void	readHandler(int fd, int eqfd, std::map<int, BufferManager>& messages);
void	writeHandler(int fd, int eqfd, std::map<int, BufferManager>& messages, const SuperServer& config);
void	establishConnection(int ev_fd, std::map<int, BufferManager> &messages, int eqfd, const SuperServer& config);
void	printClientAddress(int fd);
int		openSockets(const std::set<int>& ports, std::set<int>& sockets);
bool	isListenSocket(int fd, std::set<int>& listenSockets);
void	setFilter(int eqfd, int socket_fd, int event, int action);

#endif /* LAUNCHSERVERS_HPP */
