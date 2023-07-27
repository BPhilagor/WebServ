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
	std::map<int, BufferManager> buffer_managers;
	std::map<int, cgi_buff> cgi_messages;

#ifdef __linux__
	epoll_event events[MAX_EVENTS];
#else
	struct kevent events[MAX_EVENTS];
#endif
	while (true)
	{
		int ev_count;

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
				for(std::map<int, BufferManager>::const_iterator it =buffer_managers.begin(); it != buffer_managers.end(); ++it)
					close(it->first);
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
			try
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
				std::map<int, cgi_buff>::iterator cgi_msg = cgi_messages.find(ev_fd);
				if (cgi_msg != cgi_messages.end())
				{
					if (read_ev)
						CGIread(ev_fd, eqfd, cgi_msg, cgi_messages, buffer_managers);
					// else if (write_ev)
					// 	CGIwrite(ev_fd, eqfd, cgi_msg, cgi_messages);
				}
				else if (isListenSocket(ev_fd, config.getListeningSockets()))
				{
					establishConnection(ev_fd, buffer_managers, eqfd, config);
				}
				else if (read_ev)
				{
					readHandler(ev_fd, eqfd, buffer_managers, cgi_messages);
				}
				else if (write_ev)
				{
					writeHandler(ev_fd, eqfd, buffer_managers, config);
				}
				else
					std::cerr << ESC_COLOR_RED << "Error, event not recognized" << ESC_COLOR_RESET << std::endl;
			} catch (...)
			{
#ifdef __linux__
				std::cerr << COL(ESC_COLOR_RED , "An error occured !") << std::endl;
				close(events[i].data.fd);
				buffer_managers.erase(events[i].data.fd);
				std::cerr << "Connection closed for : "
					<< COL(ESC_COLOR_CYAN, SSTR(events[i].data.fd)) << std::endl << std::endl;
#else
				std::cerr << COL(ESC_COLOR_RED , "An error occured !") << std::endl;
				close(events[i].ident);
				buffer_managers.erase(events[i].ident);
				std::cerr << "Connection closed for : "
					<< COL(ESC_COLOR_CYAN, SSTR(events[i].ident)) << std::endl << std::endl;
#endif
			}
		}
	}
}
