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

int8_t	prepair_response(ClientNode *node);

// void	registerCGI(int fd, int eqfd, std::map<int, cgi_buff> cgi_messages)
// {
// 	// code to do this is currenly in the readHandler maybe we can refactor
// }

void	CGIread(int eqfd, ClientQueue &client_queue, ClientNode *node)
{
	/* read a chunk of the client's message into a buffer */
	char buff[BUFFER_SIZE + 1];
	int bytesRecv = read(node->cgi_fd, buff, BUFFER_SIZE);
	buff[BUFFER_SIZE] = 0;

	if (bytesRecv < BUFFER_SIZE)
	{
		if (DP_9 & DP_MASK)
		std::cout << COL(ESC_COLOR_GREEN, "CGI") << " on fd " << COL(ESC_COLOR_CYAN, node->cgi_fd) << " finished : ";

		int child_status;
		std::cout << "Wait for pid : " << node->cgi_pid << std::endl;
		if (waitpid(node->cgi_pid , &child_status, WNOHANG) == -1)
			;// TODO kill le process
		std::cout << "Finished waiting for: " << node->cgi_pid << std::endl;
		if (WIFEXITED(child_status))
		{
			std::cout<<"status: "<< COL(ESC_COLOR_CYAN, WEXITSTATUS(child_status))<<std::endl;
			if (WEXITSTATUS(child_status) != 0)
			{
				std::cerr << ESC_COLOR_RED << "Error with CGI execution !" << ESC_COLOR_RESET << std::endl;
				return;
			}
		}
		else
		{
			std::cerr<<"CGI was killed by signal: "<<WTERMSIG(child_status)<<std::endl;
			return;
		}

		if (setFilter(eqfd, node->cgi_fd, EVENT_FILTER_READ, EVENT_ACTION_DELETE)
			|| setFilter(eqfd, node->fd, EVENT_FILTER_WRITE, EVENT_ACTION_ADD, node))
			throw "Set filter error";

		if (bytesRecv < 0)
			;
		else
		{
			buff[bytesRecv] = '\0';
			node->cgi_message += buff;
		}

		prepair_response(node);
		client_queue.unsetRunningCgi(node);
		return ;
	}

	buff[bytesRecv] = '\0';
	node->cgi_message += buff;
}

int8_t	prepair_response(ClientNode *node)
{
	if (node->buffer_manager.virtual_server == NULL)
	{
		std::cout << "Host not found" << std::endl;
		// TODO : node->buffer_manager.getResponse().constructErrorReply(400);
	}
	else
	{
		// std::string			line;
		// std::istringstream	input(msg->second.cgi_msg);

		// while (true)
		// {
		// 	std::getline(input, line);
		// 	utils::sanitizeline(line);

		// 	if (line == "")
		// 		break ;

		// 	/* process header here */
		// 	std::pair<std::string, std::string> header;
		// 	utils::parseHeader(line, header);

		// 	if (msg->second.request.getAllHeaders().contains(header.first))
		// 		msg->second.request.getAllHeaders().replace(header.first, header.second);
		// 	else
		// 		msg->second.request.getAllHeaders().insert(header.first, header.second);
		// }

		// /* get what remains of the cgi_body and put it in the body */
		// if (input.tellg() > 0)
		// {
		// 	std::string body = input.str().substr(input.tellg());
		// 	msg->second.request.setBody(body);
		// 	msg->second.request.getAllHeaders().replace("Content-length", SSTR(body.size()));
		// }
		node->buffer_manager.getResponse().parseCGIResponse(node->cgi_message);
		// std::cout << "new request to pass to request worker\n" << msg->second.request << "\n";
		// requestWorker(*msg->second.virtual_server, msg->second.request, msg->second.response);
		node->buffer_manager.output_buffer = node->buffer_manager.getResponse().serialize(); // TODO : faire fonction dans buff man
	}
	return 0;
}
