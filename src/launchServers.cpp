/* ************************************************************************** */
/*                                                     __ __   __             */
/*    WebServ                                         / // / /__ \            */
/*                By: skoulen, bphilago, znichola    / // /_ __/ /            */
/*                                                  /__  __// __/             */
/*                Created: 2023/06/20 09:17:34        /_/  /____/ lausanne.ch */
/*                                                                            */
/* ************************************************************************** */

#include "launchServers.hpp"

void launchServers(const SuperServer &config)
{
	int eqfd;
#ifdef __linux__
	eqfd = epoll_create(42);
#else
	eqfd = kqueue();
#endif
	if (eqfd < 0)
	{
		std::cerr << "Fatal error when starting event queue: " << std::strerror(errno) << std::endl;
		exit(1);
	}

	std::set<int> listeningSockets;
	openSockets(config.getPorts(), listeningSockets);
	addPassiveSocketsToQueue(eqfd, listeningSockets);

	std::map<int, BufferManager> buffer_managers;

#ifdef __linux__
	epoll_event events[MAX_EVENTS];
#else
	struct kevent events[MAX_EVENTS];
#endif
	while (true)
	{
		int ev_count;
#ifdef __linux__
		ev_count = epoll_wait(eqfd, events, MAX_EVENTS, -1);
#else
		ev_count = kevent(eqfd, 0, 0, events, MAX_EVENTS, 0);
#endif
		if (ev_count < 0)
		{
			std::cerr << "Fatal error when polling for kernel events: " << strerror(errno) << std::endl;
			exit(1);
		}
		for (int i = 0; i < ev_count; i++)
		{
#ifdef __linux__
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
				establishConnection(ev_fd, buffer_managers, eqfd, config);
			}
			else if (read_ev)
			{
				readHandler(ev_fd, eqfd, buffer_managers);
			}
			else if (write_ev)
			{
				writeHandler(ev_fd, eqfd, buffer_managers, config);
			}
		}
	}
}
