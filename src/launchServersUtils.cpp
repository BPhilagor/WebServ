/* ************************************************************************** */
/*                                                     __ __   __             */
/*    WebServ                                         / // / /__ \            */
/*                By: skoulen, bphilago, znichola    / // /_ __/ /            */
/*                                                  /__  __// __/             */
/*                Created: 2023/06/20 09:17:56        /_/  /____/ lausanne.ch */
/*                                                                            */
/* ************************************************************************** */

#include "launchServers.hpp"
#include "typedefs.hpp"
#include "HTTPParser.hpp"
#include "utils.hpp"


void	addSocketToEventQueue(int eqfd, int socket_fd)
{
	#ifdef LINUX
	struct epoll_event ev;
	ev.events = EPOLLIN;
	ev.data.fd = socket_fd;
	int res = epoll_ctl(eqfd, EPOLL_CTL_ADD, socket_fd, &ev);
	#else
	struct kevent ev;
	EV_SET(&ev, socket_fd, EVFILT_READ, EV_ADD, 0, 0, NULL);
	int res = kevent(eqfd, &ev, 1, NULL, 0, NULL);
	#endif
	if (res < 0)
	{
		std::cout << "Error when adding socket events to kernel queue: " << strerror(errno) << std::endl;
		exit(1);
	}
}

void	addPassiveSocketsToQueue(int eqfd, std::set<int> listeningSockets)
{
	for (std::set<int>::iterator it = listeningSockets.begin(); it != listeningSockets.end(); ++it)
	{
		int	res;
		#ifdef LINUX
			struct epoll_event ev;
			ev.events = EPOLLIN;
			ev.data.fd = *it;
			res = epoll_ctl(eqfd, EPOLL_CTL_ADD, *it, &ev);
		#else
			struct kevent ev;
			EV_SET(&ev, *it, EVFILT_READ, EV_ADD, 0, 0, 0);
			res = kevent(eqfd, &ev, 1, 0, 0, 0);
		#endif
		if (res < 0)
		{
			std::cout << "Error when adding passive socket to queue: " << std::strerror(errno) << std::endl;
			exit(1);
		}
	}
}

void	readHandler(int fd, int eqfd, std::map<int, HTTPParser>& messages)
{
	/* read a chunk of the client's message into a buffer */
	char buff[BUFFER_SIZE + 1];
	int bytesRecv = read(fd, buff, BUFFER_SIZE);
	buff[BUFFER_SIZE] = 0;

	if (bytesRecv <= 0)
	{
		std::cout << "Client " << fd << " closed" << std::endl;
		close(fd);
		messages.erase(messages.find(fd)); /* this line can segfault if not found */
		return ;
	}

	/*
		Note that the effect of a CR (\r) char is to put the cursor at the start of the line,
		which might explain weird output!
	*/
	std::cout<< "Number of bytes received: "<<bytesRecv<<std::endl;
	std::cout << "String received: {" << std::string(buff, 0, bytesRecv) << "}" << std::endl << std::endl;

	/*
		Add the buffer to the HTTPParser, that stores the entire buffer.
		We can then interrogate the HTTPParser to find out if we've received the entire message.
	*/
	HTTPParser& parser = messages.find(fd)->second;
	parser.consumeBuffer(std::string(buff, 0, bytesRecv));
	if (parser.isFinished())
	{
		std::cout << "END OF HTTP MESSAGE DETECTED" << std::endl;
		#ifdef LINUX
			struct epoll_event ev;
			ev.events = EPOLLIN;
			ev.data.fd = socket_fd;
			epoll_ctl(eqfd, EPOLL_CTL_DEL, fd, &ev); /* handle error here */
			ev.events = EPOLLOUT;
			ev.data.fd = socket_fd;
			epoll_ctl(eqfd, EPOLL_CTL_ADD, fd, &ev);
		#else
			struct kevent ev[2];
			EV_SET(&ev[0], fd, EVFILT_READ, EV_DELETE, 0, 0, NULL);
			EV_SET(&ev[1], fd, EVFILT_WRITE, EV_ADD, 0, 0, NULL);
			kevent(eqfd, ev, 2, NULL, 0, NULL);
		#endif
	}
}

/* writeable_size is the amount of bytes that we can write */
void	writeHandler(int fd, int eqfd, std::map<int, HTTPParser>& messages)
{
	std::cout << "Detected possibility to write the socket" << std::endl;

	Data fakeData;
	HTTPParser& parser = messages.find(fd)->second;

	/* handle partial write */
	std::string& response = parser.response_buffer;
	if (parser.isFinished() && response.length() > 0) /* if we detected end of HTTPRequest, we can write the response */
	{
		std::cout << "writing..." << std::endl;
		int writtenBytes = send(fd, response.c_str(), response.length(), SEND_FLAGS);
		if (writtenBytes < 0)
		{
			std::cout << "send() failed: " << std::strerror(errno) << std::endl;
		}
		else
		{
			std::cout<<"Written bytes: " << writtenBytes << std::endl;
			response = response.substr(writtenBytes, response.length() - writtenBytes);
		}
		if (writtenBytes < 0 || response.length() == 0)
		{
			messages[fd] = HTTPParser(); /* reset the pending request */

			#ifdef LINUX
				struct epoll_event ev;
				ev.events = EPOLLIN;
				ev.data.fd = socket_fd;
				epoll_ctl(eqfd, EPOLL_CTL_ADD, fd, &ev); /* handle error here */
				ev.events = EPOLLOUT;
				ev.data.fd = socket_fd;
				epoll_ctl(eqfd, EPOLL_CTL_DEL, fd, &ev);
			#else
				struct kevent ev[2];
				EV_SET(&ev[0], fd, EVFILT_READ, EV_ADD, 0, 0, NULL);
				EV_SET(&ev[1], fd, EVFILT_WRITE, EV_DELETE, 0, 0, NULL);
				kevent(eqfd, ev, 2, NULL, 0, NULL);
			#endif
		}
	}
}

void	printClientAddress(int fd)
{
	struct sockaddr_in addr;
	socklen_t addrlen = sizeof(addr);
	getsockname(fd, (struct sockaddr *)&addr, &addrlen);
	std::cout << "Received connection from: " << inet_ntoa(addr.sin_addr) << ":" << ntohs(addr.sin_port) << std::endl;
}

// void findPorts(const Data & servers, std::set<int> &ports, mapIpPort &map_IpPort, mapPort &map_Port)
// {
// 	for (int i = 0; i < servers.count("server"); i++)
// 	{
// 		Data srv = servers.find("server", i);
// 		for (int j = 0; j < srv.count("listen"); j++)
// 		{
// 			pairIpPort ipPort = getIpPort(srv.find("listen", j).getContent());
// 			if (ipPort.first.empty())
// 				map_Port[ipPort.second].push_back(srv);
// 			else
// 				map_IpPort[ipPort].push_back(srv);
// 			ports.insert(std::atoi(ipPort.second.c_str())); /* Should check if port is in valid range maybe? */
// 		}
// 	}

// 	// Test
// 	std::cout << "Unique Ip listen" << std::endl;
// 	for(mapIpPort::const_iterator it = map_IpPort.begin(); it != map_IpPort.end(); ++it)
// 	{
// 		std::cout << it->first.first << " , " << it->first.second << " / " << it->second.size() << std::endl;
// 	}

// 	// Test
// 	std::cout << "All Ip listen" << std::endl;
// 	for(mapPort::const_iterator it = map_Port.begin();it != map_Port.end(); ++it)
// 	{
// 		std::cout << it->first << " / " << it->second.size() << std::endl;
// 	}

// 	const size_t PORTS_NBR = ports.size(); // TODO : Check si PORTS_NBR > 0 ?
// 	std::cout << "Ports detected : " << PORTS_NBR << std::endl;
// }

int	openSockets(const std::set<int>& ports, std::set<int>& sockets)
{
	// we made our sockets
	for (std::set<int>::iterator it = ports.begin(); it != ports.end(); ++it)
	{
		int socketFD = socket(AF_INET, SOCK_STREAM, 0);
		if (socketFD < 0)
		{
			std::cout << "Error when creating socket: " << std::strerror(errno) << std::endl;
			exit(1);
		}

		sockets.insert(socketFD);

		// Creating the sockaddr_in struct
		struct sockaddr_in	addr;
		memset(&addr, 0, sizeof(addr));
		addr.sin_family = AF_INET;
		addr.sin_addr.s_addr = htonl(INADDR_ANY);
		addr.sin_port = htons(*it);

		//Binding the socket with the wanted port
		if (bind(socketFD, (struct sockaddr *)&addr, sizeof(addr)))
		{
			std::cout << "Error when binding socket: " << std::strerror(errno) << std::endl;
			exit(1);
		}

		// Start listening
		if (listen(socketFD, BACK_LOG) < 0)
		{
			std::cout << "Error when establishing a listen: "<<std::strerror(errno) << std::endl;
			exit(1);
		}
		std::cout << "Socket for port : " << *it  << " created" << std::endl;
	}
	return (0);
}

bool	isListenSocket(int fd, std::set<int>& listenSockets)
{
	return (listenSockets.find(fd) != listenSockets.end());
}

void establishConnection(int ev_fd, std::map<int, HTTPParser> &messages, int eqfd)
{
	int new_socket_fd = accept(ev_fd, NULL, NULL);
	if (new_socket_fd < 0)
	{
		std::cout << "Error when accepting request: " << std::strerror(errno) << std::endl;
		exit(1); /* This might not be fatal! Better error handling needed. */
	}

#ifndef LINUX
	/* suppress SIGPIPE on the socket */
	int set = 1;
	if (setsockopt(new_socket_fd, SOL_SOCKET, SO_NOSIGPIPE, &set, sizeof(int)) != 0)
	{
		std::cout << "Error with setsockopt: " << std::strerror(errno) << std::endl;
		exit(1);
	}
#endif

	printClientAddress(new_socket_fd);
	addSocketToEventQueue(eqfd, new_socket_fd);

	/* create an HTTPParser instance for that connection */
	messages.insert(std::pair<int, HTTPParser>(new_socket_fd, HTTPParser()));
	std::cout << "Connection established on socket " << new_socket_fd << std::endl;
}
