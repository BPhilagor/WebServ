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

// static void instance_cgi_buffman(int fd, int eqfd, std::map<int, BufferManager>& messages, BufferManager& buff_man)
// {
// 	// if (setFilter(eqfd, fd, EVENT_FILTER_READ, EVENT_ACTION_DELETE))
// 	// 	throw "Set filter error";

// 	std::cout << COL(ESC_COLOR_MAGENTA, "This cgi should be added here!\n");
// 	BufferManager cgi_buffman(buff_man._config, buff_man.getResponse()._cgi_ret.fd);

// 	cgi_buffman._req = buff_man.getRequest(); // TODO (znichola) : error case when client disconects and another takes it's place quickly

// 	cgi_buffman._is_cgi_message = true;
// 	cgi_buffman._finished =false;
// 	cgi_buffman._client_fd = fd;
// 	buff_man._finished = false;

// 	messages.insert(std::pair<int, BufferManager>(cgi_buffman._fd, cgi_buffman));
// 	addSocketToEventQueue(eqfd, cgi_buffman._fd);
// }


// static void	cgi_readHandler(int fd, int eqfd, std::map<int, BufferManager>& messages)
// {
// 	BufferManager& buff_man = messages.find(fd)->second;

// 	/* read a chunk of the client's message into a buffer */
// 	char buff[BUFFER_SIZE + 1];
// 	int bytesRecv = read(fd, buff, BUFFER_SIZE);
// 	buff[BUFFER_SIZE] = 0;

// 	if (bytesRecv < BUFFER_SIZE)
// 	{
// 		if (DP_9 & DP_MASK)
// 		std::cout << "CGI Client " << COL(ESC_COLOR_CYAN, fd) << " closed" << std::endl;

// 		if (setFilter(eqfd, fd, EVENT_FILTER_READ, EVENT_ACTION_DELETE))
// 			throw "Set filter error";

// 		std::cout << "END OF CGI PARCING\n";
// 		std::cout << buff_man.getRequest();

// 		close(fd);
// 		messages.erase(messages.find(fd)); /* this line can segfault if not found */
// 		return ;
// 	}

// 	/*
// 		see readHandlaer for explication
// 	*/
// 	buff_man.addInputBuffer(std::string(buff, 0, bytesRecv));
// }

void	CGIread(int fd, int eqfd, std::map<int, cgi_buff>::iterator msg,
				std::map<int, cgi_buff> cgi_messages)
{
	/* read a chunk of the client's message into a buffer */
	char buff[BUFFER_SIZE + 1];
	int bytesRecv = read(fd, buff, BUFFER_SIZE);
	buff[BUFFER_SIZE] = 0;

	if (bytesRecv <= 0)
	{
		if (DP_9 & DP_MASK)
		std::cout << "CGI on fd " << COL(ESC_COLOR_CYAN, fd) << " finished" << std::endl;

		if (setFilter(eqfd, fd, EVENT_FILTER_READ, EVENT_ACTION_DELETE)
			|| setFilter(eqfd, msg->second.client_fd, EVENT_FILTER_WRITE, EVENT_ACTION_ADD))
			throw "Set filter error";

		std::cout << "CGI output\n" << msg->second.cgi_msg << std::endl;
		close(fd);

		return ;
	}

	buff[bytesRecv] = '\0';
	msg->second.cgi_msg += buff;

	(void)cgi_messages;
}
void	CGIwrite(int fd, int eqfd, std::map<int, cgi_buff>::iterator msg,
				std::map<int, cgi_buff> cgi_messages)
{
	(void)fd;
	(void)eqfd;
	(void)msg;
	(void)cgi_messages;
	std::cout << "we should be writing something to the correct socket here!\n";
}


std::string	prepair_resp(const std::string & cgi_ret)
{
	(void)cgi_ret;
	return "";
}
