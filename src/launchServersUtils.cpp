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

void	addSocketToEventQueue(int eqfd, int socket_fd)
{
	setFilter(eqfd, socket_fd, EVENT_FILTER_READ, EVENT_ACTION_ADD);
}

void	addPassiveSocketsToQueue(int eqfd, std::set<int> listeningSockets)
{
	for (std::set<int>::iterator it = listeningSockets.begin(); it != listeningSockets.end(); ++it)
	{
		setFilter(eqfd, *it, EVENT_FILTER_READ, EVENT_ACTION_ADD);
	}
}

void	readHandler(int fd, int eqfd, std::map<int, BufferManager>& messages)
{
	/* read a chunk of the client's message into a buffer */
	char buff[BUFFER_SIZE + 1];
	int bytesRecv = read(fd, buff, BUFFER_SIZE);
	buff[BUFFER_SIZE] = 0;

	if (bytesRecv <= 0)
	{
		if (DP_9 & DP_MASK)
		std::cout << "Client " << COL(ESC_COLOR_CYAN, fd) << " closed" << std::endl;
		close(fd);
		messages.erase(messages.find(fd)); /* this line can segfault if not found */
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
	BufferManager& buff_man = messages.find(fd)->second;
	buff_man.addInputBuffer(std::string(buff, 0, bytesRecv));
	if (buff_man.isFinished())
	{
		if (DP_2 & DP_MASK)
		std::cout << "END OF HTTP MESSAGE DETECTED" << std::endl;
		setFilter(eqfd, fd, EVENT_FILTER_READ, EVENT_ACTION_DELETE);
		setFilter(eqfd, fd, EVENT_FILTER_WRITE, EVENT_ACTION_ADD);
	}
}

void	writeHandler(int fd, int eqfd, std::map<int, BufferManager>& messages, const SuperServer& config)
{
	BufferManager& buff_man = messages.find(fd)->second;

	/* handle partial write */
	std::string& response = buff_man.output_buffer;

	int writtenBytes = send(fd, response.c_str(), response.length(), SEND_FLAGS);
	if (writtenBytes < 0)
	{
		std::cerr << "send() failed: " << std::strerror(errno) << std::endl;
	}
	else
	{
		if (DP_3 & DP_MASK)
		std::cout<<"Wrote " << writtenBytes << " bytes: " << std::endl << ESC_COLOR_CYAN << response << ESC_COLOR_RESET << std::endl;
		response = response.substr(writtenBytes, response.length() - writtenBytes);
	}
	if (writtenBytes < 0 || response.length() == 0)
	{
		std::string remaining_buffer = buff_man.input_buffer;

		/* If we announced in the header of the response that we would close the connection, we close it */
		const HTTPResponse& resp = buff_man.getResponse();
		if (resp.getHeader("Connection") == "close")
		{
			close(fd);
			messages.erase(messages.find(fd));
			return ;
		}

		buff_man = BufferManager(config, fd); /* reset the buffer manager */
		buff_man.addInputBuffer(remaining_buffer);

		setFilter(eqfd, fd, EVENT_FILTER_WRITE, EVENT_ACTION_DELETE);
		setFilter(eqfd, fd, EVENT_FILTER_READ, EVENT_ACTION_ADD);
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

int	openSockets(const std::set<int>& ports, std::set<int>& sockets)
{
	// we made our sockets
	for (std::set<int>::iterator it = ports.begin(); it != ports.end(); ++it)
	{
		int socketFD = socket(AF_INET, SOCK_STREAM, 0);
		if (socketFD < 0)
		{
			std::cerr << "Error when creating socket: " << std::strerror(errno) << std::endl;
			exit(1);
		}

		int option = 1;
		setsockopt(socketFD, SOL_SOCKET, SO_REUSEADDR, &option, sizeof(option));

		sockets.insert(socketFD);

		// Creating the sockaddr_in struct
		struct sockaddr_in	addr;
		memset(&addr, 0, sizeof(addr));
		addr.sin_family = AF_INET;
		addr.sin_addr.s_addr = htonl(INADDR_ANY);
		addr.sin_port = *it;

		//Binding the socket with the wanted port
		if (bind(socketFD, (struct sockaddr *)&addr, sizeof(addr)))
		{
			std::cerr << "Error when binding socket: " << std::strerror(errno) << std::endl;
			exit(1);
		}

		// Start listening
		if (listen(socketFD, BACK_LOG) < 0)
		{
			std::cerr << "Error when establishing a listen: "<<std::strerror(errno) << std::endl;
			exit(1);
		}
		if (DP_9 & DP_MASK)
		std::cout << "Socket for port : " << COL(ESC_COLOR_CYAN, ntohs(*it))  << " created" << std::endl;
	}
	return (0);
}

bool	isListenSocket(int fd, std::set<int>& listenSockets)
{
	return (listenSockets.find(fd) != listenSockets.end());
}

void establishConnection(int ev_fd, std::map<int, BufferManager> &messages, int eqfd, const SuperServer& config)
{
	int new_socket_fd = accept(ev_fd, NULL, NULL);
	if (new_socket_fd < 0)
	{
		std::cerr << "Error when accepting request: " << std::strerror(errno) << std::endl;
		exit(1); /* This might not be fatal! Better error handling needed. */
	}

#ifndef __linux__
	/* suppress SIGPIPE on the socket */
	int set = 1;
	if (setsockopt(new_socket_fd, SOL_SOCKET, SO_NOSIGPIPE, &set, sizeof(int)) != 0)
	{
		std::cerr << "Error with setsockopt: " << std::strerror(errno) << std::endl;
		exit(1);
	}
#endif

	printClientAddress(new_socket_fd);
	addSocketToEventQueue(eqfd, new_socket_fd);

	/* create an HTTPParser instance for that connection */
	messages.insert(std::pair<int, BufferManager>(new_socket_fd, BufferManager(config, new_socket_fd)));
	if (DP_9 & DP_MASK)
	std::cout << "Connection established on socket "
	<< COL(ESC_COLOR_CYAN, new_socket_fd) << std::endl << std::endl;
}

void	setFilter(int eqfd, int socket_fd, int event, int action)
{
#ifdef __linux__
	struct epoll_event ev;
	ev.events = (event == EVENT_FILTER_READ) ? EPOLLIN : EPOLLOUT;
	ev.data.fd = socket_fd;
	int res = epoll_ctl(eqfd, (action == EVENT_ACTION_ADD) ? EPOLL_CTL_ADD : EPOLL_CTL_DEL, socket_fd, &ev);
#else
	struct kevent ev;
	EV_SET(&ev, socket_fd, (event == EVENT_FILTER_READ) ? EVFILT_READ : EVFILT_WRITE, (action == EVENT_ACTION_ADD) ? EV_ADD : EV_DELETE, 0, 0, NULL);
	int res = kevent(eqfd, &ev, 1, NULL, 0, NULL);
#endif
	if (res < 0)
	{
		std::cerr << "Error when adding event filter to queue: " << std::strerror(errno) << std::endl;
		exit(1);
	}
}
