/* ************************************************************************** */
/*                                                     __ __   __             */
/*    WebServ                                         / // / /__ \            */
/*                By: skoulen, bphilago, znichola    / // /_ __/ /            */
/*                                                  /__  __// __/             */
/*                Created: 2023/06/27 12:03:36        /_/  /____/ lausanne.ch */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>

#include "utils.hpp"
#include "HTTPRequest.hpp"
#include "Location.hpp"
#include "debugDefs.hpp"

typedef enum {
	ws_cgi_done,
	ws_cgi_timeout
} t_cgi_state;
static t_cgi_state state = ws_cgi_done;

static void creat_env(const Location &loc,
				const HTTPRequest &req,
				const std::string &file_path,
				std::vector<std::string> &env);

static void cgiStateHandler2(int event, siginfo_t *a, void *b);

bool launchCGI(const Location &location,
				const HTTPRequest &request,
				const std::string &cgi_path,
				const std::string &file_path,
				std::string &body)
{

	if (DP_15 & DP_MASK)
	std::cout << COL(ESC_COLOR_GREEN, "launching CGI") << std::endl;

	std::vector<const char *> env;

	std::vector<std::string> tmp_env;
	creat_env(location, request, file_path, tmp_env);

	FOREACH_VECTOR(std::string, tmp_env)
	{
		env.push_back(it->c_str());
		if (DP_15 & DP_MASK)
		std::cout << it->c_str() << std::endl;
	}
	env.push_back(NULL);
	std::vector<std::string> env_data;
	
	int fd[2];
	if (pipe(fd) < 0)
	{
		std::cerr << "pipe() failed: "<< std::strerror(errno)<<std::endl;
		return false;
	}
	int pid = fork();
	if (pid < 0)
	{
		std::cerr << "fork() failed: "<<std::strerror(errno)<<std::endl;
		return false;
	}
	if (pid == 0)
	{
		FOREACH_VECTOR(std::string, env_data)
			;
		if (dup2(fd[0], STDIN_FILENO) || dup2(fd[1], STDOUT_FILENO) == -1 || close(fd[0]) == -1 || close(fd[1]) == -1)
		{
			std::cerr << "Pipe error when trying to execute : " << cgi_path << std::endl;
			exit(1);
		}
		char * const argv[3] = {const_cast<char *>(cgi_path.c_str()),
								const_cast<char *>(file_path.c_str()),
								NULL};
		execve(cgi_path.c_str(), argv, const_cast<char *const*>(&env[0]));
		std::cerr << "Error cannot execute : " << cgi_path << strerror(errno) << std::endl;
		exit(1);
	}

	int bytes_written = write(fd[1], request.getBody().c_str(), request.getBody().size());
	if (bytes_written < 0)
	{
		std::cerr << "writing request body to pipe failed: "<< std::strerror(errno) << std::endl;
		exit(1);
	}

	if (close(fd[1]) == -1)
		std::cerr << "Error when closing pipe for : " << cgi_path << std::endl;

	
	sigset_t set;
	sigemptyset(&set);
	struct sigaction sig_handler;
	sig_handler.sa_handler = 0;
	sig_handler.sa_mask = set;
	sig_handler.sa_flags = SA_RESTART;
	sig_handler.sa_sigaction = &cgiStateHandler2;
	sigaction(SIGCHLD, &sig_handler, NULL);

	waitpid(pid, NULL, 0);

	body = utils::fdToString(fd[0]);
	if (close(fd[0]) == -1)
		std::cerr << "Error when closing pipe for : " << cgi_path << std::endl;
	return true;
}

static void creat_env(const Location &loc,
				const HTTPRequest &req,
				const std::string &file_path,
				std::vector<std::string> &env)
{
	(void) loc; // TODO utiliser
	env.push_back(std::string("GATEWAY_INTERFACE=CGI/1.1"));
	env.push_back(std::string("SERVER_NAME=" + req.getHeader("host")));
	env.push_back(std::string("SERVER_SOFTWARE=WebServ_0.1"));
	env.push_back(std::string("SERVER_PROTOCOL=HTTP/1.1"));
	env.push_back(std::string("SERVER_PORT="));
	env.push_back(std::string("REQUEST_METHOD=" + utils::getMethodStr(req)));
	env.push_back(std::string("PATH_INFO=" + req.getURI().path));
	env.push_back(std::string("PATH_TRANSLATED=" + file_path));
	env.push_back(std::string("SCRIPT_NAME=" + req.getURI().path));
	env.push_back(std::string("DOCUMENT_ROOT=")); // seems to be useless
	env.push_back(std::string("QUERY_STRING=" + req.getURI().query)); // important!
	env.push_back(std::string("REMOTE_HOST="));
	env.push_back(std::string("REMOTE_ADDR="));
	env.push_back(std::string("AUTH_TYPE="));
	env.push_back(std::string("REMOTE_USER="));
	env.push_back(std::string("REMOTE_IDENT="));
	env.push_back(std::string("CONTENT_TYPE=" + req.getHeader("content-type")));
	env.push_back(std::string("CONTENT_LENGTH=" + SSTR(req.getBody().length())));
	env.push_back(std::string("HTTP_ACCEPT=" + req.getHeader("ACCEPT")));
	env.push_back(std::string("HTTP_USER_AGENT=" + req.getHeader("USER-AGENT")));
	env.push_back(std::string("HTTP_REFERER="));
	env.push_back(std::string("REDIRECT_STATUS="));
}

static void cgiStateHandler2(int event, siginfo_t *a, void *b)
{
	(void) a;
	(void) b;
	if (event == SIGALRM)
		state = ws_cgi_timeout;
	else if(event == SIGCHLD)
		state = ws_cgi_done;
}

