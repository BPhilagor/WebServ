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

class Server;

typedef std::pair<u_int32_t, u_int32_t> pairHostPort;
typedef std::map< pairHostPort, std::vector<Server> > mapHostPort;

#endif /* TYPEDEFS_HPP */
