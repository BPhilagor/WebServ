/* ************************************************************************** */
/*                                                     __ __   __             */
/*    WebServ                                         / // / /__ \            */
/*                By: skoulen, bphilago, znichola    / // /_ __/ /            */
/*                                                  /__  __// __/             */
/*                Created: 2023/06/14 15:03:11        /_/  /____/ lausanne.ch */
/*                                                                            */
/* ************************************************************************** */

#ifndef HANDLER_HPP
# define HANDLER_HPP

# include <string>

#include "Data.hpp"
#include "HTTPRequest.hpp"

std::string handler(const Data &d, const HTTPRequest &req);


#endif /* HANDLER_HPP */
