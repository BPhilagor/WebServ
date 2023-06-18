/* ************************************************************************** */
/*                                                     __ __   __             */
/*    WebServ                                         / // / /__ \            */
/*                By: skoulen, bphilago, znichola    / // /_ __/ /            */
/*                                                  /__  __// __/             */
/*                Created: 2023/06/01 23:16:37        /_/  /____/ lausanne.ch */
/*                                                                            */
/* ************************************************************************** */

#ifndef LAUNCHSERVERS_HPP
# define LAUNCHSERVERS_HPP
# define BACK_LOG 10

#ifdef WSL_DISTRO_NAME
	#include <sys/epoll.h>
#else
	#include <sys/event.h>
#endif

# include "utils.hpp"
# include "Data.hpp"
#include <set>
#include <iterator> //for std::ostream_iterator
#include <algorithm> //for std::copy
#include <iostream> //for std::cout
#include <fcntl.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <cstring>

#include "HTTPRequest.hpp"
#include "requestWorker.hpp"


#define BUFFER_SIZE 32
#define EVENTS_NBR 50

#ifdef WSL_DISTRO_NAME
	void	launchServersWSL(const Data &servers);
#else
	void	launchServersMacOS(const Data &servers);
#endif

#endif /* LAUNCHSERVERS_HPP */
