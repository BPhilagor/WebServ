/* ************************************************************************** */
/*                                                     __ __   __             */
/*    WebServ                                         / // / /__ \            */
/*                By: skoulen, bphilago, znichola    / // /_ __/ /            */
/*                                                  /__  __// __/             */
/*                Created: 2023/06/01 23:16:37        /_/  /____/ lausanne.ch */
/*                                                                            */
/* ************************************************************************** */

#include "ClientEvent.hpp"
#include "debugDefs.hpp"

ClientEvent::ClientEvent(int fd)
	: fd(fd), cgi_fd(-1), cgi_pid(-1), buffer_manager(fd)
{
	gettimeofday(&last_activity, 0);
}

ClientEvent::~ClientEvent()
{
	close(fd);
	if (cgi_fd != -1)
	{
		close(cgi_fd);
	}
	if (cgi_pid != -1)
		kill(cgi_pid, SIGKILL);
	if (DP_9 & DP_MASK)
	std::cout << "Client " << COL(ESC_COLOR_CYAN, fd) << " closed\n" << std::endl;
}
