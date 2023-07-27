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
		void		remove(ClientNode *node);
		void		append(ClientNode *node);
		ClientNode*	newNode(int fd);
		void		refresh(ClientNode *node);
};

#endif