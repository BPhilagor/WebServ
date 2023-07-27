/* ************************************************************************** */
/*                                                     __ __   __             */
/*    WebServ                                         / // / /__ \            */
/*                By: skoulen, bphilago, znichola    / // /_ __/ /            */
/*                                                  /__  __// __/             */
/*                Created: 2023/06/29 13:21:12        /_/  /____/ lausanne.ch */
/*                                                                            */
/* ************************************************************************** */

#ifndef CGI_HPP
# define CGI_HPP

#include <string>

#include "HTTPRequest.hpp"
#include "Location.hpp"
#include "utils.hpp"

cgi_ret launchCGI(const Location &location,
				const HTTPRequest &request,
				const std::string &cgi_path,
				const std::string &file_path);

#endif /* CGI_HPP */
