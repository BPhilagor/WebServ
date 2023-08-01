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
#  include <sys/wait.h>
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
# include "ClientQueue.hpp"

# define BUFFER_SIZE	1024
# define MAX_EVENTS		100
# define TIMER_PERIODE	1000


# ifdef __linux__
#  define SEND_FLAGS MSG_NOSIGNAL
# else
#  define SEND_FLAGS 0
# endif

#define EVENT_FILTER_READ	0
#define EVENT_FILTER_WRITE	1

#define EVENT_ACTION_ADD	2
#define EVENT_ACTION_DELETE	3

typedef struct
{
	int				client_fd;
	HTTPRequest		request;
	HTTPResponse	response;
	const Server *	virtual_server;
	std::string		cgi_msg;
	std::string		resp_msg;
} cgi_buff;

typedef struct
{
	int				fd;
	HTTPRequest		request;
	HTTPResponse	response;
	const Server *	virtual_server;
	int				cgi_fd;
	int				cgi_pid;
	std::string		cgi_message;
} client_event;


/* launchServers.hpp */

void    launchServers(const SuperServer &config, char **argv, char **env);

/* launchServersUtils.hpp */

void	addSocketToEventQueue(int eqfd, int socket_fd, ClientNode *node);
void	addPassiveSocketsToQueue(int eqfd, std::set<int> listeningSockets);
void	readHandler(int eqfd, ClientQueue &client_queue, ClientNode *node);
void	writeHandler(int eqfd, ClientQueue &client_queue, ClientNode *node);
void	establishConnection(int ev_fd, ClientQueue &clientQueue, int eqfd);
void	printClientAddress(int fd);
int		openSockets(const std::set<int>& ports, SuperServer &config);
bool	isListenSocket(int fd, const std::set<int>& listenSockets);
int		setFilter(int eqfd, int socket_fd, int event, int action, ClientNode *node = NULL);

/* launchServersCGI.hpp */

void	CGIread(int eqfd, ClientQueue &client_queue, ClientNode *node);

#endif /* LAUNCHSERVERS_HPP */
