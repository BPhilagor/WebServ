/* ************************************************************************** */
/*                                                     __ __   __             */
/*    WebServ                                         / // / /__ \            */
/*                By: skoulen, bphilago, znichola    / // /_ __/ /            */
/*                                                  /__  __// __/             */
/*                Created: 2023/06/20 09:17:34        /_/  /____/ lausanne.ch */
/*                                                                            */
/* ************************************************************************** */

#include "launchServers.hpp"

#ifdef __linux__
	#define	pollEvents(eqfd, events, max) epoll_wait(eqfd, events, max, -1)
#else
	#define	pollEvents(eqfd, events, max) kevent(eqfd, 0, 0, events, max, 0)
#endif

void launchServers(const SuperServer &config, char **argv, char **env)
{
	int eqfd;
#ifdef __linux__
	eqfd = epoll_create(42);
#else
	eqfd = kqueue();
#endif
	if (eqfd < 0)
	{
		std::cerr << ESC_COLOR_RED
			<< "Fatal error when starting event queue: " << std::strerror(errno)
			<< ESC_COLOR_RESET << std::endl;
		exit(1);
	}
	addPassiveSocketsToQueue(eqfd, config.getListeningSockets());

#ifdef __linux__
	epoll_event events[MAX_EVENTS]; // TODO create timer for linux version
#else
	struct kevent events[MAX_EVENTS];
	{
		struct kevent time_event;
		EV_SET(&time_event, 1024, EVFILT_TIMER, EV_ADD | EV_ENABLE, 0, TIMER_PERIODE, 0);
		kevent(eqfd, &time_event, 1, NULL, 0, NULL);
	}
#endif
	ClientQueue client_queue;

	while (true)
	{
		int ev_count;
		bool has_timer_event = false;

		ev_count = pollEvents(eqfd, events, MAX_EVENTS);

		if (ev_count < 0)
		{
			std::cerr << ESC_COLOR_RED << "Fatal error when polling for kernel events: "
				<< strerror(errno) << ESC_COLOR_RESET << std::endl;
			std::cerr << ESC_COLOR_GREEN
				<< "Retry polling event (if there's no error message after this line, consider that it worked)"
				<< ESC_COLOR_RESET << std::endl;
			ev_count = pollEvents(eqfd, events, MAX_EVENTS);
			if (ev_count < 0)
			{
				std::cerr << ESC_COLOR_RED << "Retry failed, : "
				<< strerror(errno) << "." << ESC_COLOR_RESET << std::endl;
				std::cerr << ESC_COLOR_GREEN << "Restarting server ..." << ESC_COLOR_RESET << std::endl;
				client_queue.fclear();
				for(std::set<int>::const_iterator it =config.getListeningSockets().begin(); it != config.getListeningSockets().end(); ++it)
					close(*it);
				close(eqfd);
				sleep(1);
				execve(*argv, argv, env);
				std::cerr << ESC_COLOR_RED << "Fatal error : Restart failed with error : " << strerror(errno)
					<< ESC_COLOR_RESET << std::endl;
				exit(1);
			}
			std::cerr << ESC_COLOR_GREEN << "Retry worked ! webServ continue" << ESC_COLOR_RESET << std::endl;
		}
		for (int i = 0; i < ev_count; i++)
		{
#ifdef __linux__
			bool read_ev = events[i].events & EPOLLIN;
			bool write_ev = events[i].events & EPOLLOUT;
			ClientNode *node = (ClientNode *)events[i].data.ptr; // Not fully implemented and not working
			bool is_timer_event = node == 0; // not working
			int ev_fd = events[i].data.fd; // not working
#else
			int ev_fd = events[i].ident;
			bool read_ev = events[i].filter == EVFILT_READ;
			bool write_ev = events[i].filter == EVFILT_WRITE;
			ClientNode *node = (ClientNode *)events[i].udata;
			bool is_timer_event = events[i].filter == EVFILT_TIMER;
#endif
			try
			{
				std::cout << "Event " << ev_fd << std::endl;
				if (is_timer_event)
				{
					std::cout << ESC_COLOR_RED << "TIMER !!!" << ESC_COLOR_RESET << std::endl;
					has_timer_event = true;
				}
				else if (!node && isListenSocket(ev_fd, config.getListeningSockets()))
				{
					//std::cout << "Establishing connection" << std::endl;
					establishConnection(ev_fd, client_queue, eqfd);
				}
				else if (node->cgi_fd != -1 && read_ev)
				{
					CGIread(eqfd, client_queue, node);
				}
				else if (read_ev)
				{
					//std::cout << "Node : " << node << std::endl;
					readHandler(eqfd, client_queue, node);
				}
				else if (write_ev)
				{
					writeHandler(eqfd, client_queue, node);
				}
				else
					std::cerr << ESC_COLOR_RED << "Error, event not recognized" << ESC_COLOR_RESET << std::endl;
			} catch (...)
			{
#ifdef __linux__
				std::cerr << COL(ESC_COLOR_RED , "An error occured !") << std::endl;
				client_queue.remove(node);
				std::cerr << "Connection closed for : "
					<< COL(ESC_COLOR_CYAN, SSTR(events[i].data.fd)) << std::endl << std::endl;
#else
				std::cerr << COL(ESC_COLOR_RED , "An error occured !") << std::endl;
				client_queue.remove(node);
				//client_queue.remove(node);
				std::cerr << "Connection closed for : "
					<< COL(ESC_COLOR_CYAN, SSTR(events[i].ident)) << std::endl << std::endl;
#endif
			}
		}
		if (has_timer_event)
			client_queue.removeDeadConnections();
		//client_queue.print();
	}
}
