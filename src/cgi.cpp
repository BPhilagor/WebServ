/* ************************************************************************** */
/*                                                     __ __   __             */
/*    WebServ                                         / // / /__ \            */
/*                By: skoulen, bphilago, znichola    / // /_ __/ /            */
/*                                                  /__  __// __/             */
/*                Created: 2023/06/27 12:03:36        /_/  /____/ lausanne.ch */
/*                                                                            */
/* ************************************************************************** */

#include "utils.hpp"
#include "HTTPRequest.hpp"
#include <stdlib.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>

#define UTIME_LIMIT 100000

extern char	**environ;

typedef enum {
	ws_cgi_done,
	ws_cgi_timeout
} t_cgi_state;

static t_cgi_state state = ws_cgi_done;

void cgiStateHandler(int event)
{
	if (event == SIGALRM)
		state = ws_cgi_timeout;
	else if(event == SIGCHLD)
		state = ws_cgi_done;
}

bool launchCGI(HTTPRequest request, std::string &body)
{
	char path[PATH_MAX];
	(void) request;

	// TODO : path > PATH_MAX => return

	std::vector<std::string> env_data;
	int fd[2];
	const std::string cgi_path = "/bin/cat"; // Make sure strings are null terminated

	if (pipe(fd) == -1)
		return false;

	int pid = fork();

	if (!pid)
	{
		FOREACH_VECTOR(std::string, env_data)
			;
		if (dup2(fd[1], STDOUT_FILENO) == -1 || close(fd[0]) == -1 || close(fd[1]) == -1)
		{
			std::cerr << "Pipe error when trying to execute : " << cgi_path << std::endl;
			exit(1);
		}
		strcpy(path, cgi_path.c_str());
		// char * const argv[2] = {path, NULL};
		char * const argv[2] = {const_cast<char *>(cgi_path.c_str()), NULL};
		execve(cgi_path.c_str(), argv, environ);
		std::cerr << "Error cannot execute : " << cgi_path << std::endl;
		exit(1);
	}

	if (close(fd[1]) == -1)
		std::cerr << "Error when closing pipe for : " << cgi_path << std::endl;

	signal(SIGCHLD, cgiStateHandler);
	signal(SIGALRM, cgiStateHandler);
	ualarm(UTIME_LIMIT, 0);
	pause();

	switch (state)
	{
		case ws_cgi_timeout: {
			std::cout << "CGI timed out";
			int result = waitpid(pid, NULL, WNOHANG);
			if (result == 0)
			{
				kill(pid, SIGKILL);
				waitpid(pid, NULL, 0);
				std::cout << std::endl;
			}
			else
			{
				std::cout << " but child finished normally" << std::endl;
				body = utils::fdToString(fd[0]);
			}
			if (close(fd[0]) == -1)
				std::cerr << "Error when closing pipe for : " << cgi_path << std::endl;
			return false;
		}
		case ws_cgi_done: {
			waitpid(pid, NULL, 0);
			body = utils::fdToString(fd[0]);
			if (close(fd[0]) == -1)
				std::cerr << "Error when closing pipe for : " << cgi_path << std::endl;
			return true;
		}
	}
}

