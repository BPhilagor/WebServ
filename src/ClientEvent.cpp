/* ************************************************************************** */
/*                                                     __ __   __             */
/*    WebServ                                         / // / /__ \            */
/*                By: skoulen, bphilago, znichola    / // /_ __/ /            */
/*                                                  /__  __// __/             */
/*                Created: 2023/06/01 23:16:37        /_/  /____/ lausanne.ch */
/*                                                                            */
/* ************************************************************************** */

#include "ClientEvent.hpp"

ClientEvent::ClientEvent(int fd)
	: fd(fd), cgi_fd(-1), cgi_pid(-1), buffer_manager(fd)
{
	gettimeofday(&last_activity, 0);
}

ClientEvent::~ClientEvent()
{
	std::cout << "Closing " << fd << std::endl;
	close(fd);
	if (cgi_fd != -1)
	{
		std::cout << "Closing " << cgi_fd << std::endl;
		close(cgi_fd);
	}
	if (cgi_pid != -1)
		kill(cgi_pid, SIGKILL);
	std::cout << "Client " << fd << " closed" << std::endl;
}
