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

int8_t	prepair_response(std::map<int, cgi_buff>::iterator msg);

// void	registerCGI(int fd, int eqfd, std::map<int, cgi_buff> cgi_messages)
// {
// 	// code to do this is currenly in the readHandler maybe we can refactor
// }

void	CGIread(int fd, int eqfd, std::map<int, cgi_buff>::iterator msg,
				std::map<int, cgi_buff> cgi_messages)
{
	/* read a chunk of the client's message into a buffer */
	char buff[BUFFER_SIZE + 1];
	int bytesRecv = read(fd, buff, BUFFER_SIZE);
	buff[BUFFER_SIZE] = 0;

	if (bytesRecv < BUFFER_SIZE)
	{
		if (DP_9 & DP_MASK)
		std::cout << "CGI on fd " << COL(ESC_COLOR_CYAN, fd) << " finished" << std::endl;


		if (setFilter(eqfd, fd, EVENT_FILTER_READ, EVENT_ACTION_DELETE)
			|| setFilter(eqfd, msg->second.client_fd, EVENT_FILTER_WRITE, EVENT_ACTION_ADD))
			throw "Set filter error";

		close(fd);

		if (bytesRecv < 0)
			;
		else
		{
			buff[bytesRecv] = '\0';
			msg->second.cgi_msg += buff;
		}

		std::cout << "CGI output\n" << msg->second.cgi_msg << std::endl;
		// std::cout << buff;

		prepair_response(msg);

		cgi_messages[msg->second.client_fd] = msg->second;
		std::cout << "TRYING TO SEND THIS AS RESPONSE\n" << cgi_messages[msg->second.client_fd].resp_msg << "END RESP\n";
		cgi_messages.erase(fd);
		return ;
	}

	buff[bytesRecv] = '\0';
	// std::cout << buff;
	msg->second.cgi_msg += buff;
}

void	CGIwrite(int fd, int eqfd, std::map<int, cgi_buff>::iterator msg,
				std::map<int, cgi_buff> cgi_messages)
{
	std::cout << COL(ESC_COLOR_MAGENTA, "CGI write!\n");

	std::string &response = msg->second.resp_msg;
	int writtenBytes = send(fd, response.c_str(), response.length(), SEND_FLAGS);
	if (writtenBytes < 0)
	{
		std::cerr << "send() failed: " << std::strerror(errno) << std::endl;
	}
	else
	{
		// if (DP_3 & DP_MASK)
		std::cout<<"Wrote " << writtenBytes << " bytes: " << std::endl << ESC_COLOR_CYAN << response << ESC_COLOR_RESET << std::endl;
		response = response.substr(writtenBytes, response.length() - writtenBytes);
	}
	if (writtenBytes < 0 || response.length() == 0)
	{
		cgi_messages.erase(fd);
		if (setFilter(eqfd, fd, EVENT_FILTER_WRITE, EVENT_ACTION_DELETE)
			|| setFilter(eqfd, fd, EVENT_FILTER_READ, EVENT_ACTION_ADD))
			throw "Set Filter Error";
	}
}

int8_t	prepair_response(std::map<int, cgi_buff>::iterator msg)
{
	if (msg->second.virtual_server == NULL)
	{
		std::cout << "Host not found" << std::endl;
		msg->second.response.constructErrorReply(400);
	}
	else
	{
		std::string			line;
		std::istringstream	input(msg->second.cgi_msg);

		while (true)
		{
			std::getline(input, line);
			utils::sanitizeline(line);

			if (line == "")
				break ;

			/* process header here */
			std::pair<std::string, std::string> header;
			utils::parseHeader(line, header);

			if (msg->second.request.getAllHeaders().contains(header.first))
				msg->second.request.getAllHeaders().replace(header.first, header.second);
			else
				msg->second.request.getAllHeaders().insert(header.first, header.second);
		}

		/* get what remains of the cgi_body and put it in the body */
		if (input.tellg() > 0)
		{
			std::string body = input.str().substr(input.tellg());
			msg->second.request.setBody(body);
			msg->second.request.getAllHeaders().replace("Content-length", SSTR(body.size()));
		}
		std::cout << "new request to pass to request worker\n" << msg->second.request << "\n";
		requestWorker(*msg->second.virtual_server, msg->second.request, msg->second.response);
	}
	return 0;
}
