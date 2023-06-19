#include "launchServers.hpp"

void launchServers(const Data &config)
{
	int eqfd;
#ifdef LINUX
	eqfd = epoll_create(42);
#else
	eqfd = kqueue();
#endif
	if (eqfd < 0)
	{
		std::cout << "Error when starting event queue: " << std::strerror(errno) << std::endl;
		exit(1);
	}

	std::set<int> ports;
	mapPort mapPort;
	mapIpPort mapIpPort;
	findPorts(config, ports, mapIpPort, mapPort);

	std::set<int> listeningSockets;
	openSockets(ports, listeningSockets);

	addPassiveSocketsToQueue(eqfd, listeningSockets);

	std::map<int, HTTPParser> messages;

#ifdef LINUX
	epoll_event events[MAX_EVENTS];
#else
	struct kevent events[MAX_EVENTS];
#endif
	while (true)
	{
		int ev_count;
#ifdef LINUX
		ev_count = epoll_wait(eqfd, events, MAX_EVENTS, -1);
#else
		ev_count = kevent(eqfd, 0, 0, events, MAX_EVENTS, 0);
#endif
		if (ev_count < 0)
		{
			std::cout << "Error when polling for kernel events: " << strerror(errno) << std::endl;
			exit(1);
		}
		for (int i = 0; i < ev_count; i++)
		{
#ifdef LINUX
			int ev_fd = events[i].data.fd;
			bool read_ev = events[i].events & EPOLLIN;
			bool write_ev = events[i].events & EPOLLOUT;
#else
			int ev_fd = events[i].ident;
			bool read_ev = events[i].filter == EVFILT_READ;
			bool write_ev = events[i].filter == EVFILT_WRITE;
#endif
			if (isListenSocket(ev_fd, listeningSockets))
			{
				establishConnection(ev_fd, messages, eqfd);
			}
			else if (read_ev)
			{
				readHandler(ev_fd, messages);
			}
			else if (write_ev)
			{
				writeHandler(ev_fd, messages);
			}
		}
	}
}
