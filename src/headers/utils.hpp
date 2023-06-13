/* ************************************************************************** */
/*                                                     __ __   __             */
/*    WebServ                                         / // / /__ \            */
/*                By: skoulen, bphilago, znichola    / // /_ __/ /            */
/*                                                  /__  __// __/             */
/*                Created: 2023/06/01 23:16:37        /_/  /____/ lausanne.ch */
/*                                                                            */
/* ************************************************************************** */

#ifndef UTILS_HPP
# define UTILS_HPP

# include <sys/socket.h>
# include <unistd.h>
# include <stdio.h>
# include <string.h>
# include <iostream>
# include <fstream>
# include <netinet/in.h>
# include <sstream>
# include <string>
# include <vector>

int send_to_socket(const std::string &message, int socket_fd);
int send_file_to_socket(const std::string &filename, int socket_fd);
void trim_outside_whitespace(std::string &line);

#endif
