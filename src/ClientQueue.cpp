/* ************************************************************************** */
/*                                                     __ __   __             */
/*    WebServ                                         / // / /__ \            */
/*                By: skoulen, bphilago, znichola    / // /_ __/ /            */
/*                                                  /__  __// __/             */
/*                Created: 2023/06/01 23:16:37        /_/  /____/ lausanne.ch */
/*                                                                            */
/* ************************************************************************** */

#include "ClientQueue.hpp"
#include <signal.h>

ClientQueue::ClientQueue()
	: size(0), start(0), end(0), running_cgi_start(0), running_cgi_end(0) {}
ClientQueue::~ClientQueue() {}

ClientNode::ClientNode(int fd)
	: ClientEvent(fd), next(0), prev(0) {}
ClientNode::~ClientNode() {}

void		ClientQueue::refresh(ClientNode *node)
{
	gettimeofday(&node->last_activity, 0);
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
/*
	Moves the node in cgi list and set it's last activity
*/
void	ClientQueue::setRunningCgi(ClientNode *node, int cgi_fd, int cgi_pid)
{
	node->cgi_fd = cgi_fd;
	node->cgi_pid = cgi_pid;
	gettimeofday(&node->last_activity, 0);
	//Remove from normal list
	if (node->next)
		node->next->prev = node->prev;
	else
		end = node->prev;
	if (node->prev)
		node->prev->next = node->next;
	else
		start = node->next;
	// Adding to cgi list
	if (running_cgi_start)
	{
		running_cgi_end->next = node;
		node->prev = running_cgi_end;
	} else
	{
		node->prev = 0;
		running_cgi_start = node;
	}
	running_cgi_end = node;
	node->next = 0;
}

/*
	Moves the node in normal list and set it's last activity
*/
void		ClientQueue::unsetRunningCgi(ClientNode *node)
{
	node->cgi_fd = -1;
	node->cgi_pid = -1;
	node->cgi_message.clear();
	gettimeofday(&node->last_activity, 0);
	//Remove from cgi list
	if (node->next)
		node->next->prev = node->prev;
	else
		running_cgi_end = node->prev;
	if (node->prev)
		node->prev->next = node->next;
	else
		running_cgi_start = node->next;
	// Adding to normal list
	if (start)
	{
		end->next = node;
		node->prev = end;
	} else
	{
		node->prev = 0;
		start = node;
	}
	end = node;
	node->next = 0;
}

void		ClientQueue::remove(ClientNode *node)
{
	std::cout << ESC_COLOR_GREEN << "Removing node : " << node << ESC_COLOR_RESET << std::endl;
	size -= 1;
	if (node->next)
		node->next->prev = node->prev;
	else
		end = node->prev;
	if (node->prev)
		node->prev->next = node->next;
	else
		start = node->next;
	delete node;
}

void		ClientQueue::append(ClientNode *node)
{
	std::cout << "Try to append node" << std::endl;
	size += 1;
	if (start)
	{
		end->next = node;
		node->prev = end;
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

/*
	Remove all element of the queue and close client fd
*/
// TODO : tester si tout marche bien
void		ClientQueue::fclear()
{
	std::cout << "Removing all connections" << std::endl;
	ClientNode *tmp;
	while (start)
	{
		close(start->fd);
		if (start->cgi_fd != -1)
			kill(start->cgi_pid, SIGKILL);
		tmp = start->next;
		delete tmp;
		start = tmp;
	}
	std::cout << std::endl;
}

/*
	Delete all connection older than MAX_INACTIVITY_UTIME
*/
void	ClientQueue::removeDeadConnections()
{
	ClientNode	*tmp = start;
	timeval		current_time;

	gettimeofday(&current_time, 0);
	// Remove dead connections
	while (tmp)
	{
		ClientNode *next = tmp->next;
		if ((current_time.tv_sec - tmp->last_activity.tv_sec) * MILLION + (current_time.tv_usec - tmp->last_activity.tv_usec)
			> MAX_INACTIVITY_UTIME)
		{
			std::cout << ESC_COLOR_MAGENTA << "Remove client " << tmp->fd << " for innactivity ("
				<< MAX_INACTIVITY_UTIME / MILLION << "s max)" <<  ESC_COLOR_RESET << std::endl;
			remove(tmp);
			tmp = next;
		}
		else
			break;
	}
	// Remove dead cgi
	tmp = running_cgi_start;
	while (tmp)
	{
		ClientNode *next = tmp->next;
		long running_time = (current_time.tv_sec - tmp->last_activity.tv_sec) * MILLION + (current_time.tv_usec - tmp->last_activity.tv_usec);
		std::cout << ESC_COLOR_MAGENTA << "Cgi " << tmp->fd << " running for "
				<< running_time / 1000 << " ms" <<  ESC_COLOR_RESET << std::endl;
		if (running_time > MAX_CGI_UTIME)
		{
			std::cout << ESC_COLOR_MAGENTA << "Remove cgi " << tmp->fd << " for innactivity (" << MAX_CGI_UTIME / 1000 << "ms max)" <<  ESC_COLOR_RESET << std::endl;
			// Custom remove cgi
			size -= 1;
			if (tmp->next)
				tmp->next->prev = tmp->prev;
			else
				running_cgi_end = tmp->prev;
			if (tmp->prev)
				tmp->prev->next = tmp->next;
			else
				running_cgi_start = tmp->next;
			delete tmp;
			tmp = next;
		}
		else
			break;
	}
	std::cout << std::endl;
}

void	ClientQueue::print()
{
	ClientNode *tmp = start;
	std::cout << "ClientQueue of size " << size << " : " << std::endl;
	std::cout << ESC_COLOR_CYAN << "Clients connected :" << ESC_COLOR_RESET << std::endl;
	while (tmp)
	{
		std::cout << "Client Event for fd : " << tmp->fd << " last time : " << tmp->last_activity.tv_sec
			<< ":" << tmp->last_activity.tv_usec << std::endl;
		tmp = tmp->next;
	}
	std::cout << ESC_COLOR_CYAN << "Clients connected waiting a cgi :" << ESC_COLOR_RESET << std::endl;
	tmp = running_cgi_start;
	while (tmp)
	{
		std::cout << "Client Event for fd : " << tmp->fd << " cgi " << tmp->cgi_fd << " / start time : " << tmp->last_activity.tv_sec
			<< ":" << tmp->last_activity.tv_usec << std::endl;
		tmp = tmp->next;
	}
	std::cout << std::endl;
}
