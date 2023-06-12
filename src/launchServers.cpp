/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   launchServers.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: znichola <znichola@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/12 12:25:47 by bphilago          #+#    #+#             */
/*   Updated: 2023/06/12 17:06:57 by znichola         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "launchServers.hpp"
#include <set>
#include <iterator> //for std::ostream_iterator
#include <algorithm> //for std::copy
#include <iostream> //for std::cout

void	launchServers(const Data & servers)
{

	std::vector<struct kevent> changes; /* event we want to monitor */
	std::vector<struct kevent> events; /* event that was triggered */
	int kq;

	Data listening = servers;
	std::set<int> ports;

	if ((kq = kqueue()) == -1)
	{
		std::cout << "Error: when staring kqueue" << std::endl;
		return ;
	}


	struct sockaddr_in address;
	const int addrlen = sizeof(address);

	// generat list of ports to listen too;
	for (int i = 0; i < servers.count("server"); i++)
	{
		Data srv = servers.find("server", i);
		for (int j = 0; j < srv.count("listen"); j++)
			ports.insert(srv.find("listen", j).getInt());
	}
	changes.reserve(ports.size());
	events.reserve(ports.size());
	// we made our sockets
	for (std::set<int>::iterator it = ports.begin(); it != ports.end(); ++it)
	{
		std::cout << *it << " <- listening to\n";
		int socketFD = socket(AF_INET, SOCK_STREAM, 0);
		if (socketFD < 0)
		{
			std::cout << "Error: when creating socket" << std::endl;
			return ;
		}
		// Creating the sockaddr_in struct
		memset((char *)&address, 0, sizeof(address));
		address.sin_family = AF_INET;
		address.sin_addr.s_addr = htonl(INADDR_ANY);
		address.sin_port = htons(*it);

		//Binding the socket with the wanted port
		if (bind(socketFD, (struct sockaddr *)&address, addrlen)) {
			std::cout << "Error when binding socket" << std::endl;
			return ;
		}


		struct kevent k;
		EV_SET(&k, socketFD, EVFILT_READ, EV_ADD, 0, 0, NULL);
		changes.push_back(k);



		// Start listening
		if (listen(socketFD, 10) < 0) {
			std::cout << "Error: when establishing a listen" << std::endl;
			return ;
	}
	}

	// the main program loop
	while (true)
	{

		// new_event = kevent(/*int kq, const struct kevent *changelist, size_t nchanges,
		// 	struct kevent *eventlist, size_t nevents,
		// 	const struct timespec *timeout*/);
		int event_number = kevent(kq, &changes[0], changes.size(), &events[0], changes.size(), NULL);
		std::cout << "Connection established" << std::endl;
		if (event_number < 0)
		{
			std::cout << "Error: with new_event in kq" << std::endl;
			return ;
		}
		else
		{
			for (int i = 0; i < event_number; i++)
			{
				if (events[i].flags & EV_ERROR)
				{
					fprintf(stderr, "EV_ERROR: %s\n", strerror(events[i].data));
					std::cout << "Error: with singualr event from kq" << std::endl;
					return ;
				}
				if (events[i].flags & EV_EOF)
				{
					std::cout << "End of file reached" << std::endl;
					return ;
				}
				std::cout << "dealing with the event # " << i << events[i].data << "\n";

					int new_socket = accept(events[i].ident, (struct sockaddr *)&address, (socklen_t*)&addrlen);
					if (new_socket < 0) {
						std::cout << "Error when accepting request" << std::endl;
						return ;
					}
					std::cout << "Connection established" << std::endl;

					// while receiving display message
					char buff[4096];
					while (true)
					{
						// clear buffer
						memset(buff, 0, 4096);
						// wait for message
						int bytesRecv = recv(new_socket, buff, 4096, 0);
						if (bytesRecv == -1)
						{
							std::cerr << "Error: There was a connection issue" << std::endl;
							break;
						}
						// display message
						if (bytesRecv == 0)
						{
							std::cout << "The client disconnected" << std::endl;
							break;
						}
						std::cout << "Received: " << std::string(buff, 0, bytesRecv) << std::endl;
						send(events[i].ident, buff, bytesRecv + 1, 0);
						// resend message
					}
					// close socket
					close(events[i].ident);
			}
		}
	}
}
