/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   launchServers.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bphilago <bphilago@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/12 12:25:47 by bphilago          #+#    #+#             */
/*   Updated: 2023/06/12 13:02:25 by bphilago         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <sys/event.h>
#include "utils.hpp"

void	launchServers(/*Servers List*/) {
	struct kevent change; /* event we want to monitor */
	struct kevent event; /* event that was triggered */
	int kq, new_event;
	
	if ((kq = kqueue()) == -1) {
		// Manage error
		return ;
	}
	
	for (/*All sockets*/) {
		EV_SET(/*&kev, ident, filter, flags, fflags, data, udata*/);
	}
	while (true) {
		new_event = kevent(/*int kq, const struct kevent *changelist, size_t nchanges,
			struct kevent *eventlist, size_t nevents,
			const struct timespec *timeout*/);
		if new_event < 0 {
			// Manage error
		}
		else {
			// Manage Request
		}
	}
}