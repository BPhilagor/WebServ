/* ************************************************************************** */
/*                                                     __ __   __             */
/*    WebServ                                         / // / /__ \            */
/*                By: skoulen, bphilago, znichola    / // /_ __/ /            */
/*                                                  /__  __// __/             */
/*                Created: 2023/06/15 15:18:55        /_/  /____/ lausanne.ch */
/*                                                                            */
/* ************************************************************************** */

#ifndef TYPEDEFS_HPP
# define TYPEDEFS_HPP

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <set>
#include <stdint.h>

class Server;

typedef std::pair<uint32_t, uint32_t> pairHostPort;
typedef std::map< pairHostPort, std::vector<Server> > mapHostPort;

typedef struct
{
	int	major;
	int	minor;
}	t_version;

#endif /* TYPEDEFS_HPP */
