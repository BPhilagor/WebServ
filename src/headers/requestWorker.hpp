/* ************************************************************************** */
/*                                                     __ __   __             */
/*    WebServ                                         / // / /__ \            */
/*                By: skoulen, bphilago, znichola    / // /_ __/ /            */
/*                                                  /__  __// __/             */
/*                Created: 2023/06/14 14:15:29        /_/  /____/ lausanne.ch */
/*                                                                            */
/* ************************************************************************** */

#ifndef REQUESTWORKER_HPP
# define REQUESTWORKER_HPP

# include <string>

# include "Data.hpp"
# include "HTTPRequest.hpp"

std::string requestWorker(const Data &d, int socketFD, const std::string &rawRequest);

#endif /* REQUESTWORKER_HPP */
