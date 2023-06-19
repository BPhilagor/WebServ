/* ************************************************************************** */
/*                                                     __ __   __             */
/*    WebServ                                         / // / /__ \            */
/*                By: skoulen, bphilago, znichola    / // /_ __/ /            */
/*                                                  /__  __// __/             */
/*                Created: 2023/06/01 23:16:37        /_/  /____/ lausanne.ch */
/*                                                                            */
/* ************************************************************************** */

#include "launchServers.hpp"
#include "typedefs.hpp"
#include "HTTPParser.hpp"
#include "utils.hpp"

void	readHandler(int fd, std::map<int, HTTPParser>& messages);
void	writeHandler(int fd, std::map<int, HTTPParser>& messages);
void	printClientAddress(int fd);
static void	find_ports(const Data & servers, std::set<int> &ports, mapHostPort &map_HostPort, mapPort &map_Port);
static int	open_sockets(const std::set<int>& ports, std::set<int>& sockets);

bool	isListenSocket(int fd, std::set<int>& listenSockets)
{
	return (listenSockets.find(fd) != listenSockets.end());
}

#ifdef WSL_DISTRO_NAME

int		addSocketToEventQueueLinux(int epoll_fd, int socket);

void	launchServersWSL(const Data & servers)
{
	int epollfd;

	// Finds the ports that need to be opened
	std::set<int> ports;
	mapPort map_Port;
	mapHostPort map_HostPort;
	find_ports(servers, ports, map_HostPort, map_Port);

	std::map<int, HTTPParser> messages;


	//Create the epoll
	if ((epollfd = epoll_create(42)) == -1) /* The size is ignored but must be greater than 0*/
	{
		std::cout << "Error: when starting epoll" << std::endl;
		return ;
	}

	// Creates the sockets to listen for new connections
	std::set<int> listening_sockets;

	open_sockets(ports, listening_sockets);

	/* add listening sockets to event queue */
	for (std::set<int>::iterator it = listening_sockets.begin(); it != listening_sockets.end(); ++it)
	{
		struct epoll_event	ev;
		ev.events = EPOLLIN;
		ev.data.fd = *it;
		if (epoll_ctl(epollfd, EPOLL_CTL_ADD, *it, &ev) == -1)
		{
			std::cout << "Error when adding event to epoll" << std::endl;
			exit(1);
		}
	}

	epoll_event events[MAX_EVENTS];

	// the main program loop
	while (true)
	{
		std::cout << "Waiting for something to happen on the sockets..." << std::endl;
		int event_number = epoll_wait(epollfd, events, MAX_EVENTS, -1);
		std::cout << "Something happened" << std::endl;

		if (event_number < 0)
		{
			std::cout << "Error with epoll_wait: " << std::strerror(errno) << std::endl;
			return ;
		}
		for (int i = 0; i < event_number; i++)
		{
			std::cout << "Fd on which the event occured: " << events[i].data.fd << std::endl;
			if (isListenSocket(events[i].data.fd, listening_sockets))
			{
				int new_socket_fd = accept(events[i].data.fd, NULL, NULL);
				if (new_socket_fd < 0)
				{
					std::cout << "Error when accepting request: " << std::strerror(errno) << std::endl;
					return ;
				}

				printClientAddress(new_socket_fd);

				/* add new socket to event filter */
				addSocketToEventQueueLinux(epollfd, new_socket_fd);

				messages.insert(std::pair<int, HTTPParser>(new_socket_fd, HTTPParser()));

				std::cout << "Connection established" << std::endl;
			}
			else if (events[i].events & EPOLLIN)
			{
				readHandler(events[i].data.fd, messages);
			}
			else if (events[i].events & EPOLLOUT)
			{
				writeHandler(events[i].data.fd, messages);
			}
		}
	}
}

int	addSocketToEventQueueLinux(int epoll_fd, int socket)
{
	struct epoll_event ev;
	ev.events = EPOLLIN | EPOLLOUT;
	ev.data.fd = socket;
	if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, socket, &ev) == -1)
	{
		std::cout << "Error when adding event to epoll: " << std::strerror(errno) << std::endl;
		return (-1);
	}
	return (0);
}

#else

void	addSocketToEventQueueMacOS(int kqfd, int socket, std::vector<struct kevent>& tracked);

static void process_requests_MacOS(int kqfd, std::vector<struct kevent> &tracked, std::set<int>& listening_sockets)
{
	struct kevent events[MAX_EVENTS]; /* event that was triggered */

	std::map<int, HTTPParser> messages;

	while (true)
	{
		int event_number = kevent(kqfd, &tracked[0], tracked.size(), events, MAX_EVENTS, NULL);
		if (event_number < 0)
		{
			std::cout << "Error with kevent: " << std::strerror(errno) << std::endl;
			return ;
		}
		for (int i = 0; i < event_number; i++)
		{
			struct kevent *event = &events[i];

			if (event->flags & EV_EOF) /* Client disconnected */
			{
				std::cout << "Client " << event->ident << " Disconnected" << std::endl;
				close(event->ident);
				messages.erase(messages.find(fd));
			}
			else if (event->flags & EV_ERROR) /* An error occured with the client */
			{
				std::cout << "Error with client: " << std::strerror(event->data) << std::endl;
				close(event->ident);
				messages.erase(messages.find(fd));
			}
			else if (isListenSocket(event->ident, listening_sockets)) /* Incoming connection */
			{
				int new_socket_fd = accept(event->ident, NULL, NULL);
				if (new_socket_fd < 0)
				{
					std::cout << "Error when accepting request: " << std::strerror(errno) << std::endl;
					return ;
				}

				printClientAddress(new_socket_fd);
				addSocketToEventQueueMacOS(kqfd, new_socket_fd, tracked);

				/* create an HTTPParser object for that socket */
				messages.insert(std::pair<int, HTTPParser>(new_socket_fd, HTTPParser()));
				std::cout << "Connection established" << std::endl;
			}
			else if (event->filter == EVFILT_READ) /* Socket is ready for reading */
			{
				readHandler(event->ident, messages);
				messages.erase()
			}
			else if (event->filter == EVFILT_WRITE)  /* Socket is ready for writing */
			{
				writeHandler(event->ident, messages);
			}
		}
	}
}

void	addSocketToEventQueueMacOS(int kqfd, int socket, std::vector<struct kevent>& tracked)
{
	struct kevent newEv;

	EV_SET(&newEv, new_fd, EVFILT_READ, EV_ADD, 0, 0, NULL);
	tracked.push_back(newEv);

	EV_SET(&newEv, new_fd, EVFILT_WRITE, EV_ADD, 0, 0, NULL);
	tracked.push_back(newEv);

	kevent(kqfd, &tracked[0], tracked.size(), NULL, 0, NULL);
}

void	launchServersMacOS(const Data & servers)
{
	int kqfd;
	if ((kqfd = kqueue()) == -1)
	{
		std::cout << "Error when starting kqueue: " << std::strerror(errno) << std::endl;
		exit(1);
	}

	// Finds the ports that need to be opened
	std::set<int>	ports;
	mapPort			map_Port;
	mapHostPort		map_HostPort; // those 2 maps unused yet
	find_ports(servers, ports, map_HostPort, map_Port);

	// Creates the sockets for listening
	std::set<int> listening_sockets;
	open_sockets(ports, listening_sockets);

	/* setup list of events we want to monitor on the listening sockets */
	std::vector<struct kevent> tracked;
	for (std::set<int>::iterator it = listening_sockets.begin(); it != listening_sockets.end(); ++it)
	{
		tracked.push_back({}); /* add empty struct kevent*/
		EV_SET(&tracked.back(), *it, EVFILT_READ, 0, 0, NULL);
	}

	process_requests_MacOS(kqfd, tracked, listening_sockets);
}
#endif

void	readHandler(int fd, std::map<int, HTTPParser>& messages)
{
	/* read a chunk of the client's message into a buffer */
	char buff[BUFFER_SIZE + 1];
	int bytesRecv = read(fd, buff, BUFFER_SIZE);
	buff[BUFFER_SIZE] = 0;

	if (bytesRecv <= 0)
	{
		std::cout << "Client " << fd << " closed" << std::endl;
		close(fd);
		messages.erase(messages.find(fd));
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
	}
}

void	writeHandler(int fd, std::map<int, HTTPParser>& messages)
{
	std::cout << "Detected possibility to write on the socket" << std::endl;

	Data fakeData;
	HTTPParser& parser = messages.find(fd)->second;

	if (parser.isFinished()) /* if we detected end of HTTPRequest, we can write the response */
	{
		std::string response = requestWorker(fakeData, fd, parser.getBuffer());
		write(fd, response.c_str(), response.length());
	}
}

void	printClientAddress(int fd)
{
	struct sockaddr_in addr;
	socklen_t addrlen = sizeof(addr);
	getsockname(fd, (struct sockaddr *)&addr, &addrlen);
	std::cout << "Received connection from: " << inet_ntoa(addr.sin_addr) << ":" << ntohs(addr.sin_port) << std::endl;
}

// static void find_ports(const Data & servers, std::set<int> &ports, mapHostPort &map_HostPort, mapPort &map_Port)
// {
// 	for (int i = 0; i < servers.count("server"); i++)
// 	{
// 		Data srv = servers.find("server", i);
// 		for (int j = 0; j < srv.count("listen"); j++)
// 		{
// 			pairHostPort hostPort = utils::getHostPorts(srv.find("listen", j).getContent());
// 			if (hostPort.first.empty())
// 				map_Port[hostPort.second].push_back(srv);
// 			else
// 				map_HostPort[hostPort].push_back(srv);
// 			ports.insert(std::atoi(hostPort.second.c_str())); /* Should check if port is in valid range maybe? */
// 		}
// 	}

// 	// Test
// 	std::cout << "Unique Ip listen" << std::endl;
// 	for(mapHostPort::const_iterator it = map_HostPort.begin(); it != map_HostPort.end(); ++it)
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

static int	open_sockets(const std::set<int>& ports, std::set<int>& sockets)
{
	//const int PORT_NUMBER = ports.size();

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
