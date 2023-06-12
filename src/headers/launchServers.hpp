/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   launchServers.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: znichola <znichola@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/12 14:28:36 by znichola          #+#    #+#             */
/*   Updated: 2023/06/12 14:31:00 by znichola         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LAUNCHSERVERS_HPP
# define LAUNCHSERVERS_HPP

# include <sys/event.h>

# include "utils.hpp"
# include "Data.hpp"

void	launchServers(const Data &servers);

#endif /* LAUNCHSERVERS_HPP */
