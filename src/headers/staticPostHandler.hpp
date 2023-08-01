/* ************************************************************************** */
/*                                                     __ __   __             */
/*    WebServ                                         / // / /__ \            */
/*                By: skoulen, bphilago, znichola    / // /_ __/ /            */
/*                                                  /__  __// __/             */
/*                Created: 2023/06/15 12:38:14        /_/  /____/ lausanne.ch */
/*                                                                            */
/* ************************************************************************** */

#ifndef STATIC_POST_HANDLER_H
# define STATIC_POST_HANDLER_H

# include "HTTPRequest.hpp"
# include "typedefs.hpp"
# include "utils.hpp"
# include "Location.hpp"
# include <string>
# include <vector>
# include <fcntl.h>
# include <unistd.h>
# include <sstream>
# include <fstream>
# include <iostream>
# include <sys/time.h>

int	staticPostHandler(const HTTPRequest& req, const Location& location);

#endif
