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
	: fd(fd), last_activity(clock()), virtual_server(0)
{
	cgi_fd = 0;
	cgi_pid = 0;
}

ClientEvent::~ClientEvent()
{
	close(fd);
};