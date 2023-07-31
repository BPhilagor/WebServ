/* ************************************************************************** */
/*                                                     __ __   __             */
/*    WebServ                                         / // / /__ \            */
/*                By: skoulen, bphilago, znichola    / // /_ __/ /            */
/*                                                  /__  __// __/             */
/*                Created: 2023/06/29 13:21:12        /_/  /____/ lausanne.ch */
/*                                                                            */
/* ************************************************************************** */


#ifndef CLIENT_EVENT_HPP
# define CLIENT_EVENT_HPP
# include <sys/time.h>
# include "signal.h"
# include "HTTPRequest.hpp"
# include "HTTPResponse.hpp"
# include "Server.hpp"
# include "BufferManager.hpp"

typedef struct timeval t_timeval;

class ClientEvent
{
	public :
		ClientEvent(int fd);
		~ClientEvent();

		int				fd;
		t_timeval		last_activity;
		int				cgi_fd;
		int				cgi_pid;
		std::string		cgi_message;
		BufferManager	buffer_manager;
};

#endif
