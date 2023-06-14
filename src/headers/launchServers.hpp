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

#ifdef WSL_DISTRO_NAME
	void	launchServersWSL(const Data &servers);
#else
	void	launchServersMacOS(const Data &servers);
#endif

#endif /* LAUNCHSERVERS_HPP */
