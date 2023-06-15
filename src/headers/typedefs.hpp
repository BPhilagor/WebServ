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
#include "Data.hpp"

typedef std::pair<std::string, std::string> pairIpPort;
typedef std::map< pairIpPort, std::vector<Data> > mapIpPort;
typedef std::map< std::string, std::vector<Data> > mapPort;

#endif /* TYPEDEFS_HPP */
