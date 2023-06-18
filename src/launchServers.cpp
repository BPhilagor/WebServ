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

typedef struct {
	int fd;
	struct sockaddr_in address;
} socket_event;

void	readHandler(int fd, std::map<int, HTTPParser>& messages);
void	writeHandler(int fd, std::map<int, HTTPParser>& messages);

static void find_ports(const Data & servers, std::set<int> &ports, mapIpPort &map_IpPort, mapPort &map_Port)
{
	for (int i = 0; i < servers.count("server"); i++)
	{
		Data srv = servers.find("server", i);
		for (int j = 0; j < srv.count("listen"); j++)
		{
			pairIpPort ipPort = getIpPort(srv.find("listen", j).getContent());
			if (ipPort.first.empty())
				map_Port[ipPort.second].push_back(srv);
			else
				map_IpPort[ipPort].push_back(srv);
			ports.insert(std::atoi(ipPort.second.c_str())); /* Should check if port is in valid range maybe? */
		}
	}
}

static socket_event* open_sockets(const std::set<int> ports)
{
	const int PORT_NUMBER = ports.size();;
	socket_event* socket_events = (socket_event*) operator new[](sizeof(socket_event) * PORT_NUMBER); // TODO : manage memory error
	size_t index = 0;

	// we made our sockets
	for (std::set<int>::iterator it = ports.begin(); it != ports.end(); ++it)
	{
		int socketFD = socket(AF_INET, SOCK_STREAM, 0);
		if (socketFD < 0)
		{
			std::cout << "Error when creating socket: " << std::strerror(errno) << std::endl;
			delete[] socket_events;
			return 0;
		}
		// Adding event to the List
		socket_events[index].fd = socketFD;

		// Creating the sockaddr_in struct
		memset((char *)&socket_events[index].address, 0, sizeof(socket_events[index].address));
		socket_events[index].address.sin_family = AF_INET;
		socket_events[index].address.sin_addr.s_addr = htonl(INADDR_ANY);
		socket_events[index].address.sin_port = htons(*it);

		//Binding the socket with the wanted port
		if (bind(socketFD, (struct sockaddr *)&socket_events[index].address, sizeof(sockaddr_in))) {
			std::cout << "Error when binding socket: " << std::strerror(errno) << std::endl;
			delete[] socket_events;
			return 0;
		}

		// Start listening
		if (listen(socketFD, BACK_LOG) < 0) {
			std::cout << "Error: when establishing a listen" << std::endl;
			delete[] socket_events;
			return 0;
		}
		++index;
		std::cout << "Socket for port : " << *it  << " created" << std::endl;
	}
	return socket_events;
}

#ifdef WSL_DISTRO_NAME

bool	isListenSocket(int fd, const socket_event* sockets, size_t size)
{
	std::cout << "Sockets for listening: ";
	for (unsigned int i = 0; i < size; i++)
	{
		std::cout << sockets[i].fd << " ";
	}
	std::cout << std::endl;
	for (unsigned int i = 0; i < size; i++)
	{
		if (fd == sockets[i].fd)
			return true;
	}
	return false;
}

void	launchServersWSL(const Data & servers)
{
	int epollfd;
	const int ADDRLEN = sizeof(sockaddr_in);

	// Finds the ports that need to be opened
	std::set<int> ports;
	mapPort map_Port;
	mapIpPort map_IpPort;
	find_ports(servers, ports, map_IpPort, map_Port);
	
	const size_t PORTS_NBR = ports.size(); // TODO : Check si PORTS_NBR > 0 ?
	std::cout << "Ports detected : " << PORTS_NBR << std::endl;


	std::map<int, HTTPParser> messages;


	//Create the epoll
	if ((epollfd = epoll_create(42)) == -1) /* The size is ignored but must be greater than 0*/
	{
		std::cout << "Error: when starting epoll" << std::endl;
		return ;
	}

	// Creates the sockets to listen for new connections
	socket_event* listening_sockets = open_sockets(ports);
	if (listening_sockets == NULL)
	{
		std::cout << "Error: when creating sockets" << std::endl;
		return ;
	}

	for (size_t i = 0; i < PORTS_NBR; ++i)
	{
		struct epoll_event	ev;

		ev.events = EPOLLIN | EPOLLOUT;
		ev.data.ptr = &listening_sockets[i];

		if (epoll_ctl(epollfd, EPOLL_CTL_ADD, listening_sockets[i].fd, &ev) == -1) {
			std::cout << "Error when adding event to epoll" << std::endl;
		}
	}

	std::vector<epoll_event> events;
	events.resize(PORTS_NBR);

	// the main program loop
	while (true)
	{
		std::cout << "Waiting for something to happen on the sockets..." << std::endl;
		int event_number = epoll_wait(epollfd, &events[0], 10, -1);
		std::cout << "Something happened" << std::endl;

		if (event_number < 0)
		{
			std::cout << "Error with epoll_wait: " << std::strerror(errno) << std::endl;
			return ;
		}
		for (int i = 0; i < event_number; i++)
		{
			socket_event se = *(socket_event*) events[i].data.ptr;
			std::cout << "Fd on which the event occured: " << se.fd << std::endl;
			if (isListenSocket(se.fd, listening_sockets, PORTS_NBR))
			{
				int new_socket = accept(se.fd, (struct sockaddr *)&se.address, (socklen_t*)&ADDRLEN);
				if (new_socket < 0) {
					std::cout << "Error when accepting request: " << std::strerror(errno) << std::endl;
					return ;
				}

				/* add new socket to event filter */
				
				struct epoll_event ev;
				se.fd = new_socket;
				ev.events = EPOLLIN | EPOLLOUT;
				ev.data.ptr = &se;

				if (epoll_ctl(epollfd, EPOLL_CTL_ADD, se.fd, &ev) == -1) {
					std::cout << "Error when adding event to epoll: " << std::strerror(errno) << std::endl;
				}

				messages.insert(std::pair<int, HTTPParser>(se.fd, HTTPParser()));

				std::cout << "Connection established" << std::endl;
			}
			else if (events[i].events & EPOLLIN)
			{
				readHandler(se.fd, messages);
			}
			else if (events[i].events & EPOLLOUT)
			{
				writeHandler(se.fd, messages);
			}
		}
	}
}

#else

static void process_requests_MacOS(int kqfd, std::vector<struct kevent> &tracked, std::set<int> &socket_fds)
{
	const int ADDRLEN = sizeof(sockaddr_in);
	//const size_t PORTS_NBR = tracked.size();
	struct kevent events[EVENTS_NBR]; /* event that was triggered */
	struct kevent newEv;

	std::map<int, HTTPParser> messages;

	while (true)
	{
		int event_number = kevent(kqfd, &tracked[0], tracked.size(), events, EVENTS_NBR, NULL);
		if (event_number < 0)
		{
			std::cout << "Error: with new_event in kq" << std::endl;
			return ;
		}
		for (int i = 0; i < event_number; i++)
		{
			struct kevent *event = &events[i];

			if (event->flags & EV_EOF) /* Client disconnected */
			{
				std::cout << "Client " << event->ident << " Disconnected" << std::endl;
				close(event->ident);
			}
			else if (event->flags & EV_ERROR) /* An error occured with the client */
			{
				std::cout << "Error with client: " << std::strerror(event->data) << std::endl;
				close(event->ident);
			}
			else if (socket_fds.find(event->ident) != socket_fds.end()) /* Incoming connection */
			{
				socket_event *se = (socket_event*) event->udata;
				int new_fd = accept(se->fd, (struct sockaddr *)&se->address, (socklen_t*)&ADDRLEN);
				if (new_fd < 0) {
					std::cout << "Error when accepting request: " << std::strerror(errno) << std::endl;
					return ;
				}

				// check the socket ip:port config
				struct sockaddr_in addr;
				socklen_t addrlen = sizeof(addr);
				getsockname(new_fd, (struct sockaddr *)&addr, &addrlen);
				std::string ipAddr(inet_ntoa(addr.sin_addr));
				std::cout << "here:" << inet_ntoa(addr.sin_addr) << ":" << ntohs(addr.sin_port) << "end\n";

				EV_SET(&newEv, new_fd, EVFILT_READ, EV_ADD, 0, 0, NULL);
				tracked.push_back(newEv);

				EV_SET(&newEv, new_fd, EVFILT_WRITE, EV_ADD, 0, 0, NULL);
				tracked.push_back(newEv);

				kevent(kqfd, &tracked[0], tracked.size(), NULL, 0, NULL); /* Subscribe to the event queue */

				messages.insert(std::pair<int, HTTPParser>(new_fd, HTTPParser()));
				std::cout << "Connection established" << std::endl;
			}
			else if (event->filter == EVFILT_READ) /* Socket is ready for reading */
			{
				readHandler(event->ident, messages);
			}
			else if (event->filter == EVFILT_WRITE)  /* Socket is ready for writing */
			{
				writeHandler(event->ident, messages);
			}
		}
	}
}

void	launchServersMacOS(const Data & servers)
{

	int kqfd;

	if ((kqfd = kqueue()) == -1)
	{
		std::cout << "Error when starting kqueue: " << std::strerror(errno) << std::endl;
		return ;
	}

	// Finds the ports that need to be opened
	std::set<int> ports;
	mapPort map_Port;
	mapIpPort map_IpPort;
	find_ports(servers, ports, map_IpPort, map_Port);

	// Test
	std::cout << "Unique Ip listen" << std::endl;
	for(mapIpPort::const_iterator it = map_IpPort.begin(); it != map_IpPort.end(); ++it)
	{
		std::cout << it->first.first << " , " << it->first.second << " / " << it->second.size() << std::endl;
	}

	// Test
	std::cout << "All Ip listen" << std::endl;
	for(mapPort::const_iterator it = map_Port.begin();it != map_Port.end(); ++it)
	{
		std::cout << it->first << " / " << it->second.size() << std::endl;
	}

	const size_t PORTS_NBR = ports.size(); // TODO : Check si PORTS_NBR > 0 ?
	std::cout << "Ports detected : " << PORTS_NBR << std::endl;

	// Creates the sockets
	socket_event* socket_events = open_sockets(ports);
	if (socket_events == 0)
	{
		std::cout << "Error when creating sockets: " << std::strerror(errno) << std::endl;
		return ;
	}

	std::vector<struct kevent> tracked; /* event we want to monitor */
	tracked.resize(PORTS_NBR * 2);

	std::set<int> socket_fds;

	// Add events to tracked list
	for (size_t i = 0; i < PORTS_NBR * 2; i += 2)
	{
		socket_fds.insert(socket_events[i/2].fd);
		EV_SET(&tracked[i], socket_events[i/2].fd, EVFILT_READ, EV_ADD, 0, 0, &socket_events[i/2]);
		EV_SET(&tracked[i + 1], socket_events[i/2].fd, EVFILT_WRITE, EV_ADD, 0, 0, &socket_events[i/2]);
	}

	for (unsigned int i = 0; i < tracked.size(); i++)
	{
		std::cout << "( " <<tracked[i].filter <<", " << tracked[i].ident <<" )" << std::endl;
	}
	process_requests_MacOS(kqfd, tracked, socket_fds);
}
#endif

void	readHandler(int fd, std::map<int, HTTPParser>& messages)
{
	/* read a chunk of the client's message into a buffer */
	char buff[BUFFER_SIZE + 1];
	int bytesRecv = read(fd, buff, BUFFER_SIZE);
	buff[BUFFER_SIZE] = 0;


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
		messages.erase(messages.find(fd));
		close(fd);
		std::cout << "Client " << fd << " closed" << std::endl;
	}
}
