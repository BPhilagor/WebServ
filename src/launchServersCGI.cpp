/* ************************************************************************** */
/*                                                     __ __   __             */
/*    WebServ                                         / // / /__ \            */
/*                By: skoulen, bphilago, znichola    / // /_ __/ /            */
/*                                                  /__  __// __/             */
/*                Created: 2023/07/26 16:59:06        /_/  /____/ lausanne.ch */
/*                                                                            */
/* ************************************************************************** */

#include "utils.hpp"
#include "debugDefs.hpp"
#include "launchServers.hpp"

void	CGIread(int eqfd, ClientQueue &client_queue, ClientNode *node)
{
	/* read a chunk of the client's message into a buffer */
	char buff[BUFFER_SIZE + 1];
	int bytesRecv = read(node->cgi_fd, buff, BUFFER_SIZE);
	buff[BUFFER_SIZE] = 0;
	bool cgi_exit_sucess = false;
	if (bytesRecv <= 0)
	{
		if (DP_9 & DP_MASK)
		std::cout << COL(ESC_COLOR_GREEN, "CGI") << " on fd " << COL(ESC_COLOR_CYAN, node->cgi_fd) << " finished : ";

		int child_status;
		if (!waitpid(node->cgi_pid , &child_status, WNOHANG)) // Pipe is closed but pro
			return ;
		if (WIFEXITED(child_status))
		{
			std::cout<<"status: "<< COL(ESC_COLOR_CYAN, WEXITSTATUS(child_status))<<std::endl;
			if (WEXITSTATUS(child_status) != 0)
				std::cerr << ESC_COLOR_RED << "Error with CGI execution !" << ESC_COLOR_RESET << std::endl;
			else
				cgi_exit_sucess = true;
		}
		else
			std::cerr << "CGI was killed by signal: " << WTERMSIG(child_status) << std::endl;

		if (setFilter(eqfd, node->cgi_fd, EVENT_FILTER_READ, EVENT_ACTION_DELETE)
			|| setFilter(eqfd, node->fd, EVENT_FILTER_WRITE, EVENT_ACTION_ADD, node))
			throw "Set filter error";

		if (cgi_exit_sucess)
			node->buffer_manager.getResponse().parseCGIResponse(node->cgi_message);
		else
			node->buffer_manager.getResponse().constructErrorReply(500);

		node->buffer_manager.getResponse().finalize(node->buffer_manager.getRequest());
		node->buffer_manager.output_buffer = node->buffer_manager.getResponse().serialize();
		//std::cout << ESC_COLOR_CYAN << node->buffer_manager.output_buffer << ESC_COLOR_RESET << std::endl;
		client_queue.unsetRunningCgi(node);
		return ;
	}

	buff[bytesRecv] = '\0';
	node->cgi_message += buff;
}
