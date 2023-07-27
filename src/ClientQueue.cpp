/* ************************************************************************** */
/*                                                     __ __   __             */
/*    WebServ                                         / // / /__ \            */
/*                By: skoulen, bphilago, znichola    / // /_ __/ /            */
/*                                                  /__  __// __/             */
/*                Created: 2023/06/01 23:16:37        /_/  /____/ lausanne.ch */
/*                                                                            */
/* ************************************************************************** */

#include "ClientQueue.hpp"

ClientQueue::ClientQueue()
	: size(0), start(0), end(0) {}
ClientQueue::~ClientQueue() {}

ClientNode::ClientNode(int fd)
	: ClientEvent(fd), next(0), prev(0) {}
ClientNode::~ClientNode() {}

void		ClientQueue::refresh(ClientNode *node)
{
	node->last_activity = clock();
	if (node->next)
		node->next->prev = node->prev;
	else
		return ;

	if (node->prev)
		node->prev->next = node->next;
	else
		start = node->next;

	node->prev = end;
	node->next = 0;
	end = node;
}

void		ClientQueue::remove(ClientNode *node)
{
	size -= 1;
	if (node->next)
		node->next->prev = node->prev;
	else
		end = 0;
	if (node->prev)
		node->prev->next = node->next;
	else
	{
		start = 0;
		end = 0;
	}
	delete node;
}
void		ClientQueue::append(ClientNode *node)
{
	size += 1;
	if (start)
	{
		end->next = node;
		node->prev = node;
	} else
	{
		node->prev = 0;
		start = node;
	}
	end = node;
	node->next = 0;
}

ClientNode*	ClientQueue::newNode(int fd)
{
	ClientNode *newEvent = new ClientNode(fd);
	this->append(newEvent);
	return newEvent;
}

void	ClientQueue::print()
{
	ClientNode *tmp = start;
	std::cout << "ClientQueue of size " << size << " :" << std::endl;
	while (tmp)
	{
		std::cout << "Client Event for fd : " << tmp->fd << std::endl;
		tmp = tmp->next;
	}
	std::cout << std::endl;
}
