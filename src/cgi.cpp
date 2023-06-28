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
#include <string.h>
#include "Location.hpp"

#define UTIME_LIMIT 10000000

typedef enum {
	ws_cgi_done,
	ws_cgi_timeout
} t_cgi_state;

static t_cgi_state state = ws_cgi_done;

void creat_env(const Location &loc,
				const HTTPRequest &req,
				std::vector<std::string> &env)
{
	(void) loc; // TODO utiliser
	env.push_back(std::string("GATEWAY_INTERFACE="));
	env.push_back(std::string("SERVER_NAME="));
	env.push_back(std::string("SERVER_SOFTWARE=WebServ_0.1"));
	env.push_back(std::string("SERVER_PROTOCOL="));
	env.push_back(std::string("SERVER_PORT="));
	env.push_back(std::string("REQUEST_METHOD=" + utils::getMethodStr(req)));
	env.push_back(std::string("PATH_INFO="));
	env.push_back(std::string("PATH_TRANSLATED="));
	env.push_back(std::string("SCRIPT_NAME=" + req.getURI().path));
	env.push_back(std::string("DOCUMENT_ROOT="));
	env.push_back(std::string("QUERY_STRING=" + req.getURI().query)); // important!
	env.push_back(std::string("REMOTE_HOST="));
	env.push_back(std::string("REMOTE_ADDR="));
	env.push_back(std::string("AUTH_TYPE="));
	env.push_back(std::string("REMOTE_USER="));
	env.push_back(std::string("REMOTE_IDENT="));
	env.push_back(std::string("CONTENT_TYPE=text/php")); // hardcoded for now
	env.push_back(std::string("CONTENT_LENGTH=" + SSTR(req.getBody().length())));
	env.push_back(std::string("HTTP_ACCEPT=" + req.getHeader("ACCEPT")));
	env.push_back(std::string("HTTP_USER_AGENT=" + req.getHeader("USER-AGENT")));
	env.push_back(std::string("HTTP_REFERER="));
	env.push_back(std::string("REDIRECT_STATUS="));
}


void cgiStateHandler(int event)
{
	if (event == SIGALRM)
		state = ws_cgi_timeout;
	else if(event == SIGCHLD)
		state = ws_cgi_done;
}

bool launchCGI(HTTPRequest request, std::string &body)
{
	std::vector<const char *> env;

	Location tmp;
	std::vector<std::string> tmp_env;
	creat_env(tmp, request, tmp_env);

	FOREACH_VECTOR(std::string, tmp_env)
	{
		env.push_back(it->c_str());
	}
	env.push_back(NULL);
	std::vector<std::string> env_data;
	int fd[2];
	const std::string cgi_path = "cgi/php-cgi"; // Make sure strings are null terminated

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
		char * const argv[2] = {const_cast<char *>(cgi_path.c_str()), NULL};
		execve(cgi_path.c_str(), argv, const_cast<char *const*>(&env[0]));
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

int main(void)
{
	HTTPRequest tmp;
	std::string body;

	if (launchCGI(tmp, body)) {
		std::cout << "Body : " << std::endl << body << std::endl;
	} else {
		std::cerr << "Error with CGI" << std::endl;
	}
}
