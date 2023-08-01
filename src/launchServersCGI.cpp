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

void	CGIread(int fd, int eqfd, std::map<int, cgi_buff>::iterator msg,
				std::map<int, cgi_buff> &cgi_messages, std::map<int, BufferManager> &buffer_managers)
{
	/* read a chunk of the client's message into a buffer */
	char buff[BUFFER_SIZE + 1];
	int bytesRecv = read(fd, buff, BUFFER_SIZE);
	buff[BUFFER_SIZE] = 0;
	bool cgi_exit_sucess = false;
	if (bytesRecv <= 0)
	{
		if (DP_9 & DP_MASK)
		std::cout << COL(ESC_COLOR_GREEN, "CGI") << " on fd " << COL(ESC_COLOR_CYAN, fd) << " finished : ";

		int child_status;
		waitpid(msg->second.response._cgi_ret.pid , &child_status, 0);
		if (WIFEXITED(child_status))
		{
			std::cout<<"status: "<< COL(ESC_COLOR_CYAN, WEXITSTATUS(child_status))<<std::endl;
			if (WEXITSTATUS(child_status) != 0)
				std::cerr << ESC_COLOR_RED << "Error with CGI execution !" << ESC_COLOR_RESET << std::endl;
			else
				cgi_exit_sucess = true;
		}
		else
			std::cerr<<"CGI was killed by signal: "<<WTERMSIG(child_status)<<std::endl;

		if (setFilter(eqfd, fd, EVENT_FILTER_READ, EVENT_ACTION_DELETE)
			|| setFilter(eqfd, msg->second.client_fd, EVENT_FILTER_WRITE, EVENT_ACTION_ADD))
			throw "Set filter error";

		close(fd);

		if (buffer_managers.find(msg->second.client_fd) == buffer_managers.end())
		{
			PERR2("Fd not found for sending message !, fd : ", fd);
			return ;
		}
		BufferManager & bufMan = buffer_managers.find(msg->second.client_fd)->second;

		if (cgi_exit_sucess)
			bufMan.getResponse().parseCGIResponse(msg->second.cgi_msg);
		else
			bufMan.getResponse().constructErrorReply(500);

		bufMan.getResponse().finalize();
		bufMan.output_buffer = bufMan.getResponse().serialize();
		cgi_messages.erase(fd);
		return ;
	}

	buff[bytesRecv] = '\0';
	msg->second.cgi_msg += buff;
}
