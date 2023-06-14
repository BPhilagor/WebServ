/* ************************************************************************** */
/*                                                     __ __   __             */
/*    WebServ                                         / // / /__ \            */
/*                By: skoulen, bphilago, znichola    / // /_ __/ /            */
/*                                                  /__  __// __/             */
/*                Created: 2023/06/01 23:16:37        /_/  /____/ lausanne.ch */
/*                                                                            */
/* ************************************************************************** */

#include "launchServers.hpp"

typedef struct {
	int fd;
	struct sockaddr_in address;
} socket_event;

static void find_ports(const Data & servers, std::set<int> &ports)
{
	for (int i = 0; i < servers.count("server"); i++)
	{
		Data srv = servers.find("server", i);
		for (int j = 0; j < srv.count("listen"); j++)
			ports.insert(srv.find("listen", j).getInt());
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
void	launchServersWSL(const Data & servers)
{
	int epollfd;
	const int ADDRLEN = sizeof(sockaddr_in);

	// Finds the ports that need to be opened
	std::set<int> ports;
	find_ports(servers, ports);
	const size_t PORTS_NBR = ports.size(); // TODO : Check si PORTS_NBR > 0 ?
	std::cout << "Ports detected : " << PORTS_NBR << std::endl;

	//Create the epoll
	if ((epollfd = epoll_create(PORTS_NBR)) == -1)
	{
		std::cout << "Error: when staring kqueue" << std::endl;
		return ;
	}

	// Creates the sockets
	socket_event* socket_events = open_sockets(ports, ADDRLEN);
	if (socket_events == 0)
	{
		std::cout << "Error: when creating sockets" << std::endl;
		return ;
	}

	for (size_t i = 0; i < PORTS_NBR; ++i)
	{
		struct epoll_event ev;

		ev.events = EPOLLIN;
		ev.data.ptr = &socket_events[i];
		if (epoll_ctl(epollfd, EPOLL_CTL_ADD, socket_events[i].fd, &ev) == -1) {
			std::cout << "Error when adding event to epoll" << std::endl;
		}
	}

	std::vector<epoll_event> events;
	events.resize(PORTS_NBR);

	// the main program loop
	while (true)
	{
		// new_event = kevent(/*int kq, const struct kevent *changelist, size_t nchanges,
		// 	struct kevent *eventlist, size_t nevents,
		// 	const struct timespec *timeout*/);
		std::cout << "Waiting for connection" << std::endl;
		int event_number = epoll_wait(epollfd, &events[0], 10, -1);
		std::cout << "Connection established" << std::endl;
		if (event_number < 0)
		{
			std::cout << "Error: with new_event in kq" << std::endl;
			return ;
		}
		else
		{
			for (int i = 0; i < event_number; i++)
			{
				socket_event *se = (socket_event*) events[i].data.ptr;
				int new_socket = accept(se->fd, (struct sockaddr *)&se->address, (socklen_t*)&ADDRLEN);
				if (new_socket < 0) {
					std::cout << "Error when accepting request" << std::endl;
					return ;
				}
				std::cout << "Connection established" << std::endl;

				// while receiving display message
				char buff[4096];
				while (true)
				{
					// clear buffer
					memset(buff, 0, 4096);
					// wait for message
					int bytesRecv = recv(new_socket, buff, 4096, 0);
					if (bytesRecv == -1)
					{
						std::cerr << "Error: There was a connection issue" << std::endl;
						break;
					}
					// display message
					std::cout << "Byte NBR : " << bytesRecv << std::endl;
					if (bytesRecv == 0)
					{
						std::cout << "The client disconnected" << std::endl;
						break;
					}
					std::cout << "Received: " << std::string(buff, 0, bytesRecv) << std::endl;
					send(events[i].data.fd, buff, bytesRecv + 1, 0);
					// resend message
				}
				// close socket
				close(new_socket);
			}
		}
	}
}

#else

static void process_requests_MacOS(int kqfd, std::vector<struct kevent> &tracked, std::set<int> &socket_fds)
{
	const int ADDRLEN = sizeof(sockaddr_in);
	const size_t PORTS_NBR = tracked.size();
	struct kevent events[EVENTS_NBR]; /* event that was triggered */
	struct kevent newEv;
	std::map<int, std::string> messages;

	while (true)
	{
		// new_event = kevent(/*int kq, const struct kevent *changelist, size_t nchanges,
		// 	struct kevent *eventlist, size_t nevents,
		// 	const struct timespec *timeout*/);
		int event_number = kevent(kqfd, &tracked[0], PORTS_NBR, events, EVENTS_NBR, NULL);
		if (event_number < 0)
		{
			std::cout << "Error: with new_event in kq" << std::endl;
			return ;
		}
		for (int i = 0; i < event_number; i++)
		{
			struct kevent *event = &events[i];

			if (event->flags & EV_EOF)
			{
				std::cout << "Client " << event->ident << " Disconnected" << std::endl;
				EV_SET(&newEv, event->ident, EVFILT_READ, EV_DELETE, 0, 0, NULL);
				kevent(kqfd, &newEv, 1, NULL, 0, NULL);
				EV_SET(&newEv, event->ident, EVFILT_WRITE, EV_ADD | EV_ONESHOT, 0, 0, NULL);
				kevent(kqfd, &newEv, 1, NULL, 0, NULL);
			}
			else if (event->flags & EV_ERROR)
			{
				std::cout << "Error with client" << std::endl;
				close(event->ident);
			}
			else if (socket_fds.find(event->ident) != socket_fds.end())
			{
				if (event->flags & EV_ERROR)
				{
					fprintf(stderr, "EV_ERROR: %s\n", strerror(event->data));
					std::cout << "Error: with singualr event from kq" << std::endl;
					return ;
				}

				socket_event *se = (socket_event*) event->udata;
				int new_fd = accept(se->fd, (struct sockaddr *)&se->address, (socklen_t*)&ADDRLEN);
				if (new_fd < 0) {
					std::cout << "Error when accepting request: " << std::strerror(errno) << std::endl;
					return ;
				}
				EV_SET(&newEv, new_fd, EVFILT_READ, EV_ADD, 0, 0, NULL);
				kevent(kqfd, &newEv, 1, NULL, 0, NULL);
				messages.insert(std::pair<int, std::string>(new_fd, ""));
				std::cout << "Connection established" << std::endl;
			}
			else if (event->filter == EVFILT_READ)
			{
				char buff[BUFFER_SIZE];
				int bytesRecv = read(event->ident, buff, BUFFER_SIZE);
				std::cout << "Received : " << std::string(buff, 0, bytesRecv) << std::endl;
				messages.find(event->ident)->second += std::string(buff, 0, bytesRecv);
			}
			else if (event->filter == EVFILT_WRITE)
			{
				Data fakeData;
				std::map<int, std::string>::iterator message = messages.find(event->ident);
				std::string response = requestWorker(fakeData, event->ident, message->second);
				write(event->ident, message->second.c_str(), message->second.length());
				messages.erase(message);
				close(event->ident);
				std::cout << "Client " << event->ident << " closed" << std::endl;
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
	find_ports(servers, ports);
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
	tracked.resize(PORTS_NBR);

	std::set<int> socket_fds;

	// Add events to tracked list
	for (size_t i = 0; i < PORTS_NBR; ++i)
	{
		socket_fds.insert(socket_events[i].fd);
		EV_SET(&tracked[i], socket_events[i].fd, EVFILT_READ, EV_ADD, 0, 0, &socket_events[i]);
	}

	process_requests_MacOS(kqfd, tracked, socket_fds);
}
#endif
