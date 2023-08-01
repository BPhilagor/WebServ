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

static void creat_env(const Location &loc,
				const HTTPRequest &req,
				const std::string &file_path,
				std::vector<std::string> &env);

cgi_ret launchCGI(const Location &location,
				const HTTPRequest &request,
				const std::string &cgi_path,
				const std::string &file_path)
{
	if (DP_15 & DP_MASK)
	std::cout << COL(ESC_COLOR_GREEN, "launching CGI ") << cgi_path << std::endl;

	cgi_ret ret;

	ret.fd = -1;
	ret.pid = 0;

	std::vector<const char *> env;

	std::vector<std::string> tmp_env;
	creat_env(location, request, file_path, tmp_env);

	FOREACH_VECTOR(std::string, tmp_env)
	{
		env.push_back(it->c_str());
		// if (DP_16 & DP_MASK)
		// std::cout << it->c_str() << std::endl;
	}
	env.push_back(NULL);

	int fd[2];
	if (pipe(fd) < 0)
	{
		std::cerr << "pipe() failed: "<< std::strerror(errno)<<std::endl;
		return ret;
	}
	int pid = fork();
	if (pid < 0)
	{
		std::cerr << "fork() failed: "<<std::strerror(errno)<<std::endl;
	}
	if (pid == 0)
	{
		//sleep(3);
		std::string dirname = utils::getDirname(file_path);
		std::cerr << "New working directory: "<<dirname <<std::endl;
		if (chdir(dirname.c_str()) != 0)
		{
			std::cerr << ESC_COLOR_RED << "Failed to change to directory "<<dirname<<" : "<<std::strerror(errno)<<std::endl;
			exit(1);
		}
		if (dup2(fd[0], STDIN_FILENO) || dup2(fd[1], STDOUT_FILENO) == -1 || close(fd[0]) == -1 || close(fd[1]) == -1)
		{
			std::cerr << ESC_COLOR_RED << "Pipe error when trying to execute : "
				<< cgi_path << ESC_COLOR_RESET << std::endl;
			exit(1);
		}

		std::cerr << "Executing script: "<<file_path<<std::endl;

		char * const argv[3] = {const_cast<char *>(cgi_path.c_str()),
								const_cast<char *>(file_path.c_str()),
								NULL};
		if (DP_16 & DP_MASK)
		std::cerr << "CGI args:\"" << argv[0] << "\", \""	<< argv[1] << "\"\n" << std::endl;
		execve(cgi_path.c_str(), argv, const_cast<char *const*>(&env[0]));
		std::cerr << ESC_COLOR_RED << "Error cannot execute : " << cgi_path
			<< strerror(errno) << ESC_COLOR_RESET << std::endl;
		exit(1);
	}

	if (DP_16 & DP_MASK)
	std::cerr << "CGI writing body to it's stdin\n";
	int bytes_written = write(fd[1], request.getBody().c_str(), request.getBody().size());
	if (bytes_written < 0)
	{
		std::cerr << "writing request body to pipe failed: "<< std::strerror(errno) << std::endl;
		return ret;
	}

	if (close(fd[1]) == -1)
	{
		std::cerr << "Error when closing pipe for : " << cgi_path << std::endl;
		return ret;
	}
	if (DP_16 & DP_MASK)
	std::cerr << "CGI finished with cgi process\n";
	ret.fd = fd[0];
	ret.pid = pid;
	return ret;
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

	//is the part that comes after the name of the script
	//ex:	if the url is /index.php/truc/bidule
	//		and index.php is a cgi script
	//		then the PATH_INFO is /truc/bidule
	env.push_back(std::string("PATH_INFO="));


	env.push_back(std::string("PATH_TRANSLATED=" + file_path));
	env.push_back(std::string("SCRIPT_NAME=" + req.getURI().path));
	env.push_back(std::string("DOCUMENT_ROOT=!!!! this should be set to the root of the website i.e. websites/python-website/")); // seems to be useless
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
	env.push_back(std::string("HTTP_COOKIE=" + req.getHeader("Cookie")));
	env.push_back(std::string("POTATO=trucbidule"));
}
