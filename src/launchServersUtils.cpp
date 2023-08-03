/* ************************************************************************** */
/*                                                     __ __   __             */
/*    WebServ                                         / // / /__ \            */
/*                By: skoulen, bphilago, znichola    / // /_ __/ /            */
/*                                                  /__  __// __/             */
/*                Created: 2023/06/20 09:17:56        /_/  /____/ lausanne.ch */
/*                                                                            */
/* ************************************************************************** */

#include "launchServers.hpp"
#include "debugDefs.hpp"
#include "utils.hpp"

void	addSocketToEventQueue(int eqfd, int socket_fd, ClientNode *node)
{
	setFilter(eqfd, socket_fd, EVENT_FILTER_READ, EVENT_ACTION_ADD, node);
}

void	addPassiveSocketsToQueue(int eqfd, const std::set<int> listeningSockets)
{
	for (std::set<int>::const_iterator it = listeningSockets.begin(); it != listeningSockets.end(); ++it)
	{
		if (setFilter(eqfd, *it, EVENT_FILTER_READ, EVENT_ACTION_ADD))
		{
			std::cerr << ESC_COLOR_RED << "Fatal error when creating passive socket event"
				<< ESC_COLOR_RESET << std::endl;
			exit (1);
		}
	}
}

void	readHandler(int eqfd, ClientQueue &client_queue, ClientNode *node)
{
	/* read a chunk of the client's message into a buffer */
	client_queue.refresh(node);
	char buff[BUFFER_SIZE + 1];
	int bytesRecv = read(node->fd, buff, BUFFER_SIZE);
	buff[BUFFER_SIZE] = 0;

	if (bytesRecv <= 0)
	{
		client_queue.remove(node);
		return ;
	}

	/*
		Note that the effect of a CR (\r) char is to put the cursor at the start of the line,
		which might explain weird output!
	*/
	if (DP_1 & DP_MASK)
	std::cout << "Received "<< bytesRecv<< " bytes: "<< std::endl << ESC_COLOR_MAGENTA << std::string(buff, 0, bytesRecv) << ESC_COLOR_RESET << std::endl << std::endl;

	/*
		Add the buffer to the HTTPParser, that stores the entire buffer.
		We can then interrogate the HTTPParser to find out if we've received the entire message.
	*/
	node->buffer_manager.addInputBuffer(std::string(buff, 0, bytesRecv));
	if (node->buffer_manager.isFinished())
	{
		if (DP_2 & DP_MASK)
		std::cout << "END OF HTTP MESSAGE DETECTED" << std::endl;

		if (node->buffer_manager.getResponse().is_cgi_used)
		{
			client_queue.setRunningCgi(node, node->buffer_manager.getResponse()._cgi_ret.fd,
				node->buffer_manager.getResponse()._cgi_ret.pid); // TODO : simplifier
			if (setFilter(eqfd, node->fd, EVENT_FILTER_READ, EVENT_ACTION_DELETE)
			|| setFilter(eqfd, node->cgi_fd, EVENT_FILTER_READ, EVENT_ACTION_ADD, node))
			throw "Set filter error";
		}
		else if (setFilter(eqfd, node->fd, EVENT_FILTER_READ, EVENT_ACTION_DELETE)
			|| setFilter(eqfd, node->fd, EVENT_FILTER_WRITE, EVENT_ACTION_ADD, node))
			throw "Set filter error";
	}
}

void	writeHandler(int eqfd, ClientQueue &client_queue, ClientNode *node)
{
	/* handle partial write */
	client_queue.refresh(node);
	std::string& response = node->buffer_manager.output_buffer;

	int writtenBytes = send(node->fd, response.c_str(), response.length(), SEND_FLAGS);
	if (writtenBytes < 0)
	{
		std::cerr << ESC_COLOR_RED << "send() failed: " << std::strerror(errno) << ESC_COLOR_RESET << std::endl;
	}
	else
	{
		//if (DP_3 & DP_MASK)
		//std::cout<<"Wrote " << writtenBytes << " bytes: " << std::endl << ESC_COLOR_CYAN << response << ESC_COLOR_RESET << std::endl;
		response = response.substr(writtenBytes, response.length() - writtenBytes);
	}
	if (writtenBytes < 0 || response.length() == 0)
	{
		std::string remaining_buffer = node->buffer_manager.input_buffer;

			if (DP_14 & DP_MASK)
			std::cout << COL(ESC_COLOR_MAGENTA, SSTR(node->buffer_manager.getResponse().getCode()))
			<< " sent for client " << COL(ESC_COLOR_CYAN, SSTR(node->fd))
			<< " request for " << COL(ESC_COLOR_CYAN, node->buffer_manager.getRequest().getURI().path)
			<< std::endl << std::endl;

		/* If we announced in the header of the response that we would close the connection, we close it */
		if (node->buffer_manager.getRequest().getHeader("Connection") == "close")
		{
			client_queue.remove(node);
			return ;
		}

		node->buffer_manager = BufferManager(node->fd); /* reset the buffer manager */
		node->buffer_manager.addInputBuffer(remaining_buffer);

		if (setFilter(eqfd, node->fd, EVENT_FILTER_WRITE, EVENT_ACTION_DELETE)
			|| setFilter(eqfd, node->fd, EVENT_FILTER_READ, EVENT_ACTION_ADD, node))
			throw "Set Filter Error";
	}
}

void	printClientAddress(int fd)
{
	struct sockaddr_in addr;
	socklen_t addrlen = sizeof(addr);
	getsockname(fd, (struct sockaddr *)&addr, &addrlen);
	if (DP_9 & DP_MASK)
	std::cout << "Received connection from: "
	<< COL(ESC_COLOR_CYAN ,inet_ntoa(addr.sin_addr)) << ":"
	<< COL(ESC_COLOR_CYAN , ntohs(addr.sin_port)) << std::endl;
}

static int openSocket(SuperServer &config, int port_nbr)
{
	int socketFD = socket(AF_INET, SOCK_STREAM, 0);
	if (socketFD < 0)
	{
		std::cerr << ESC_COLOR_RED << "Error when creating socket: "
			<< std::strerror(errno) << ESC_COLOR_RESET << std::endl;
		exit(1);
	}

	int option = 1;
	setsockopt(socketFD, SOL_SOCKET, SO_REUSEADDR, &option, sizeof(option));

	// Creating the sockaddr_in struct
	struct sockaddr_in	addr;
	memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = htonl(INADDR_ANY);
	addr.sin_port = port_nbr;

	//Binding the socket with the wanted port
	if (bind(socketFD, (struct sockaddr *)&addr, sizeof(addr)))
	{
		std::cerr << "Error when binding socket: port "
		<< COL(ESC_COLOR_CYAN, ntohs(port_nbr)) << " .. "
		<< std::strerror(errno) << std::endl;
		std::cout << "Trying using: $> netstat -vanp tcp | grep port_number" << std::endl;
		close(socketFD);
		return (1);
	}

	// Start listening
	if (listen(socketFD, BACK_LOG) < 0)
	{
		std::cerr << ESC_COLOR_RED << "Error when establishing a listen: "
		<< std::strerror(errno) << ESC_COLOR_RESET << std::endl;
		exit(1);
	}
	if (DP_9 & DP_MASK)
		std::cout << "Socket for port : " << COL(ESC_COLOR_CYAN, ntohs(port_nbr))  << " created" << std::endl;
	config.addListeningSocket(socketFD);
	return (0);
}

int	openSockets(const std::set<int>& ports, SuperServer &config)
{
	// we made our sockets
	for (std::set<int>::iterator it = ports.begin(); it != ports.end(); ++it)
	{
		while (openSocket(config, *it))
		{
			if (errno == EADDRINUSE)
			{
				std::cerr << ESC_COLOR_GREEN << "Retrying in 5 seconds ..." << ESC_COLOR_RESET << std::endl;
				sleep(5);
			} else
				exit (1);
		}
	}
	return (0);
}

bool	isListenSocket(int fd, const std::set<int>& listenSockets)
{
	return (listenSockets.find(fd) != listenSockets.end());
}

void establishConnection(int ev_fd, ClientQueue &client_queue, int eqfd)
{
	int new_socket_fd = accept(ev_fd, NULL, NULL);
	if (new_socket_fd < 0)
	{
		std::cerr << ESC_COLOR_RED << "Error when accepting request: "
			<< std::strerror(errno) << ESC_COLOR_RESET << std::endl;
		return ;
	}
	ClientNode *newNode = client_queue.newNode(new_socket_fd);

#ifndef __linux__
	/* suppress SIGPIPE on the socket */
	int set = 1;
	if (setsockopt(new_socket_fd, SOL_SOCKET, SO_NOSIGPIPE, &set, sizeof(int)) != 0)
	{
		std::cerr << ESC_COLOR_RED << "Error with setsockopt: "
			<< std::strerror(errno) << ESC_COLOR_RESET << std::endl;
		client_queue.remove(newNode);
		return ;
	}
#endif

	printClientAddress(new_socket_fd);
	addSocketToEventQueue(eqfd, new_socket_fd, newNode);

	if (DP_9 & DP_MASK)
		std::cout << "Connection established on socket "
		<< COL(ESC_COLOR_CYAN, new_socket_fd) << std::endl << std::endl;
}

int	setFilter(int eqfd, int socket_fd, int event, int action, ClientNode *node)
{
#ifdef __linux__
	(void) socket_fd;
	struct epoll_event ev;
	ev.events = (event == EVENT_FILTER_READ) ? EPOLLIN : EPOLLOUT;
	ev.data.ptr = node;
	int res = epoll_ctl(eqfd, (action == EVENT_ACTION_ADD) ? EPOLL_CTL_ADD : EPOLL_CTL_DEL, socket_fd, &ev);
#else
	struct kevent ev;
	EV_SET(&ev, socket_fd, (event == EVENT_FILTER_READ) ? EVFILT_READ : EVFILT_WRITE, (action == EVENT_ACTION_ADD) ? EV_ADD : EV_DELETE, 0, 0, node);
	int res = kevent(eqfd, &ev, 1, NULL, 0, NULL);
#endif
	if (res < 0)
	{
		std::cerr << ESC_COLOR_RED << "Error when adding event filter to queue: "
			<< std::strerror(errno) << ESC_COLOR_RESET << std::endl;
		return (1);
	}
	return (0);
}
