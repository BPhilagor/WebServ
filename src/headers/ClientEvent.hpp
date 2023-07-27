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
# include <chrono>
# include "HTTPRequest.hpp"
# include "HTTPResponse.hpp"
# include "Server.hpp"


class ClientEvent
{
	public :
		ClientEvent(int fd);
		~ClientEvent();

		int				fd;
		clock_t			last_activity;
		HTTPRequest		request;
		HTTPResponse	response;
		const Server	*virtual_server;
		int				cgi_fd;
		int				cgi_pid;
		std::string		cgi_message;
};

#endif