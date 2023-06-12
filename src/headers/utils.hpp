/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bphilago <bphilago@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/11 15:24:53 by bphilago          #+#    #+#             */
/*   Updated: 2023/06/12 13:09:17 by bphilago         ###   ########.fr       */
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
# include <unordered_map>

int send_to_socket(const std::string &message, int socket_fd);
int send_file_to_socket(const std::string &filename, int socket_fd);

#endif

