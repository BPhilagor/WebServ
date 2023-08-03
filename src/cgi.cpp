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
#include "typedefs.hpp"

static int open_pipe(int *fd);
static int close_pipe(int *fd);

inline static int fork_cgi(int *server_cgi, int *cgi_server, const std::vector<char *> &env,
				const std::string &cgi_path, const std::string &file_path);

static std::vector<std::string> create_env(const Location &loc,
				const HTTPRequest &req,
				const std::string &file_path);

static std::vector<char *> convert_env(std::vector<std::string> &string_env);

cgi_ret launchCGI(const Location &location,
				const HTTPRequest &request,
				const std::string &cgi_path,
				const std::string &file_path)
{
	if (DP_15 & DP_MASK)
		std::cout << COL(ESC_COLOR_GREEN, "launching CGI ") << cgi_path << std::endl;
	int							server_cgi[2];
	int							cgi_server[2];
	cgi_ret						ret;
	std::vector<std::string> string_env = create_env(location, request, file_path);
	std::vector<char*>	cstring_env = convert_env(string_env);
	ret.fd = -1;
	ret.pid = -1;

	// Opening pipes
	if (open_pipe(server_cgi) == -1)
		return ret;
	if (open_pipe(cgi_server) == -1)
	{
		close_pipe(server_cgi);
		return ret;
	}

	// Launching CGI
	ret.fd = cgi_server[0];
	ret.pid = fork_cgi(server_cgi, cgi_server, cstring_env, cgi_path, file_path);

	// Parent Process
	if (DP_16 & DP_MASK)
		std::cerr << "CGI writing body to it's stdin\n";
	// Send body to child
	int bytes_written = write(server_cgi[1], request.getBody().c_str(), request.getBody().size());
	if (bytes_written < 0)
	{
		std::cerr << "Writing request body to pipe failed: "<< std::strerror(errno) << std::endl;
		return ret;
	}

	// Closing unused pipes
	close_pipe(server_cgi);
	close(cgi_server[1]);
	if (DP_16 & DP_MASK)
		std::cerr << "CGI finished with cgi process\n";
	return ret;
}

static int open_pipe(int *fd)
{
	if (pipe(fd) < 0)
	{
		std::cerr << "pipe() failed: "<< std::strerror(errno)<<std::endl;
		return 1;
	}
	return 0;
}

static int close_pipe(int *fd)
{
	int ret_value = 0;
	if (close(fd[0]) == -1)
	{
		std::cerr << ESC_COLOR_RED << "Error when closing pipe " << fd[0] << ESC_COLOR_RESET << std::endl;
		ret_value = 1;
	}
	if (close(fd[1]) == -1)
	{
		std::cerr << ESC_COLOR_RED << "Error when closing pipe " << fd[1] << ESC_COLOR_RESET << std::endl;
		ret_value = 1;
	}
	return ret_value;
}

inline static int fork_cgi(int *server_cgi, int *cgi_server, const std::vector<char *> &env,
				const std::string &cgi_path, const std::string &file_path)
{
	int pid = fork();
	if (pid < 0)
	{
		std::cerr << "fork() failed: "<<std::strerror(errno)<<std::endl;
		return -1;
	}
	if (pid == 0)
	{
		//sleep(3);
		std::string dirname = utils::getDirname(file_path);
		if (DP_16 & DP_MASK)
			std::cout << "New working directory: "<<dirname <<std::endl;
		if (chdir(dirname.c_str()) != 0)
		{
			std::cerr << ESC_COLOR_RED << "Failed to change to directory "<<dirname<<" : "<<std::strerror(errno)<<std::endl;
			exit(1);
		}
		if (dup2(server_cgi[0], STDIN_FILENO) == -1 || dup2(cgi_server[1], STDOUT_FILENO) == -1
			|| close(server_cgi[1]) == -1 || close(cgi_server[0]) == -1)
		{
			std::cerr << ESC_COLOR_RED << "Pipe error when trying to execute : "
				<< cgi_path << ESC_COLOR_RESET << std::endl;
			exit(1);
		}

		if (DP_16 & DP_MASK)
			std::cerr << "Executing script: "<<file_path<<std::endl;

		char * const argv[3] = {const_cast<char *>(cgi_path.c_str()),
								const_cast<char *>(file_path.c_str()),
								NULL};
		// if (DP_16 & DP_MASK)
			std::cerr << "CGI args:\"" << argv[0] << "\", \""	<< argv[1] << "\"\n" << std::endl;
		// std::cerr << "exeve args:{\n" << argv[0] << "\n" << argv[1] << "\n}\n";
		std::cerr << "env:{\n";
		FOREACH_VECTOR(const char *, env)
		{
			if (it != env.end() - 1)
				std::cerr << "{"<< *it << "}\n";
		}
		std::cerr << "\n}\n";
		execve(cgi_path.c_str(), argv, env.data());
		std::cerr << ESC_COLOR_RED << "Error cannot execute : " << cgi_path
			<< strerror(errno) << ESC_COLOR_RESET << std::endl;
		exit(1);
	}
	return pid;
}

static std::vector<std::string> create_env(const Location &loc,
				const HTTPRequest &req,
				const std::string &file_path)
{
	(void) loc; // TODO utiliser ?
	std::vector<std::string> string_env;

	string_env.push_back(std::string("GATEWAY_INTERFACE=CGI/1.1"));
	string_env.push_back(std::string("SERVER_NAME=" + req.getHeader("host")));
	string_env.push_back(std::string("SERVER_SOFTWARE=WebServ_0.1"));
	string_env.push_back(std::string("SERVER_PROTOCOL=HTTP/1.1"));
	string_env.push_back(std::string("SERVER_PORT="));
	string_env.push_back(std::string("REQUEST_METHOD=" + utils::getMethodStr(req)));

	//is the part that comes after the name of the script
	//ex:	if the url is /index.php/truc/bidule
	//		and index.php is a cgi script
	//		then the PATH_INFO is /truc/bidule
	string_env.push_back(std::string("PATH_INFO="));


	string_env.push_back(std::string("PATH_TRANSLATED=" + file_path));
	string_env.push_back(std::string("SCRIPT_NAME=" + req.getURI().path));
	string_env.push_back(std::string("DOCUMENT_ROOT=!!!! this should be set to the root of the website i.e. websites/python-website/")); // seems to be useless
	string_env.push_back(std::string("QUERY_STRING=" + req.getURI().query)); // important!
	string_env.push_back(std::string("REMOTE_HOST="));
	string_env.push_back(std::string("REMOTE_ADDR="));
	string_env.push_back(std::string("AUTH_TYPE="));
	string_env.push_back(std::string("REMOTE_USER="));
	string_env.push_back(std::string("REMOTE_IDENT="));
	string_env.push_back(std::string("CONTENT_TYPE=" + req.getHeader("content-type")));
	string_env.push_back(std::string("CONTENT_LENGTH=" + SSTR(req.getBody().length())));
	string_env.push_back(std::string("HTTP_ACCEPT=" + req.getHeader("ACCEPT")));
	string_env.push_back(std::string("HTTP_USER_AGENT=" + req.getHeader("USER-AGENT")));
	string_env.push_back(std::string("HTTP_REFERER="));
	string_env.push_back(std::string("REDIRECT_STATUS="));
	string_env.push_back(std::string("HTTP_COOKIE=" + req.getHeader("Cookie")));
	string_env.push_back(std::string("POTATO=trucbidule")); // THIS IS FINE (said skoulen)

	return string_env;
}

static std::vector<char *> convert_env(std::vector<std::string> &string_env)
{
	std::vector<char*> cstring_env;

	// Converting vector of string into vector of const char *
	for(std::vector<std::string>::iterator it = string_env.begin(); it != string_env.end(); ++it)
		cstring_env.push_back(&it->front());
	cstring_env.push_back(NULL);
	return cstring_env;
}
