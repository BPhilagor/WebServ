/* ************************************************************************** */
/*                                                     __ __   __             */
/*    WebServ                                         / // / /__ \            */
/*                By: skoulen, bphilago, znichola    / // /_ __/ /            */
/*                                                  /__  __// __/             */
/*                Created: 2023/06/20 09:17:34        /_/  /____/ lausanne.ch */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_QUEUE_HPP
# define CLIENT_QUEUE_HPP
# define MAX_INACTIVITY_UTIME 10000000
# define MAX_CGI_UTIME 2000000
# define MILLION 1000000

#include "ClientEvent.hpp"

class ClientNode : public ClientEvent
{
	public :
		ClientNode(int fd);
		~ClientNode();
		ClientNode *next;
		ClientNode *prev;
};



class ClientQueue
{
	public :
		ClientQueue();
		~ClientQueue();
		size_t		size;
		ClientNode	*start;
		ClientNode	*end;
		ClientNode	*running_cgi_start;
		ClientNode	*running_cgi_end;
		void		setRunningCgi(ClientNode *node, int cgi_fd, int cgi_pid);
		void		unsetRunningCgi(ClientNode *node);
		void		remove(ClientNode *node);
		ClientNode*	newNode(int fd);
		void		refresh(ClientNode *node);
		void		removeDeadConnections();
		void		fclear();
		void		print() const;
	private :
		void	appendCGI(ClientNode *node);
		void	append(ClientNode *node);
		void	unlinkCGI(ClientNode *node);
		void	unlink(ClientNode *node);

};

#endif
