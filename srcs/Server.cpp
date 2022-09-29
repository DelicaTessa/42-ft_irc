/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   Server.cpp                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: tessaclement <tessaclement@student.coda      +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/08/24 09:31:55 by tessaclemen   #+#    #+#                 */
/*   Updated: 2022/08/26 11:45:23 by tessaclemen   ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/Server.hpp"

Server::Server(int port, std::string const & pass) : port(port), pass(pass), command(pass, clients, IP)
{
	findHostname();
	command.setIP(IP);
}

Server::Server() : port(4242), pass(""), command(pass, clients, IP)
{
	findHostname();
	command.setIP(IP);
}

Server::~Server()
{
	close(head_socket);
}

void	Server::findHostname()
{
	char	host[256];
	struct hostent	*host_entry;
	gethostname(host, sizeof(host));
	host_entry = gethostbyname(host);
	IP = inet_ntoa(*((struct in_addr *)host_entry->h_addr_list[0]));
}

void	Server::initialize()
{
	int	opt = 1;

	if( (head_socket = socket(AF_INET , SOCK_STREAM , 0)) == 0)  
		throw std::runtime_error("socket");

	if(setsockopt(head_socket, SOL_SOCKET, SO_REUSEADDR, (char *)&opt, sizeof(opt)) < 0)  
		throw std::runtime_error("setsockopt");

	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons(port);

	if (fcntl(head_socket, F_SETFL, O_NONBLOCK) < -1)
		throw std::runtime_error("fcntl");

	if (bind(head_socket, (struct sockaddr *)&address, sizeof(address))<0)  
		throw std::runtime_error("bind failed");
	if (VERBOSE)
		std::cout << CYAN + UNDER + "Listening on port: " << port << " ; password is: " << pass << RESET << std::endl;
		
	if (listen(head_socket, 3) < 0)  
		throw std::runtime_error("listen");
	addrlen = sizeof(address);
	if (VERBOSE)
		std::cout << CYAN + UNDER + "Waiting for connection........" + RESET << std::endl << std::endl;
}

void	Server::launch()
{
	int	sd;
	int	max;
	int	res;
	std::list<Client>::iterator	it;
	char	buf[2048];

	while(true)
	{
		FD_ZERO(&readfds);
		FD_SET(head_socket, &readfds);  
		max = head_socket;
		for (it = clients.begin(); it != clients.end(); it++)
		{
			if (it->getbeDeleted())
				it = clients.erase(it);
			sd = it->getSocket();		
			FD_SET(sd, &readfds);
			if (sd > max)
				max = sd;
		}
		res = select(max + 1 , &readfds , NULL , NULL , NULL);  
		if ((res < 0) && (errno!=EINTR))  
			throw std::runtime_error("select error");
		if (FD_ISSET(head_socket, &readfds))  
		{
			if ((res = accept(head_socket, (struct sockaddr *)&address, (socklen_t*)&addrlen))<0)  
				throw std::runtime_error("accept");
			Client	client(res, inet_ntoa(address.sin_addr), ntohs(address.sin_port));
			std::string	message(":" + IP + " NOTICE * :IRC : To register please use commands PASS - NICK - USER(user, mode, unused, realname)\r\n");
			send(client.getSocket(), message.c_str(), message.length(), 0);
			if (VERBOSE)
				std::cout << UNDER + "New connection, socket fd : " << client.getSocket() << ", IP : " << client.getIP() << ", port : " << client.getPort() << RESET << std::endl;
			clients.push_back(client);
		}
		for (it = clients.begin(); it != clients.end(); it++)
		{
			sd = it->getSocket();
			if (FD_ISSET(sd, &readfds))
			{
				if ((res = recv(sd, (void*)buf, 2048, 0)) == 0)
				{
					it->setConnected(false);
					it->getBuff().assign("QUIT :Remote host closed the connection\r\n");
				}
				else
				{
					buf[res] = '\0';
					it->getBuff().append(buf);
				}
				if (it->getBuff()[it->getBuff().length() - 1] == '\n')
				{
					if (VERBOSE)
					{
						std::cout << "Command received, socket fd : " << it->getSocket() << ", IP : " << it->getIP() << ", port : " << it->getPort() << std::endl;
						std::cout << BLUE + "<< " + it->getBuff() + RESET;
					}
					command.parsCmd(*it);
					it->getBuff().clear();
				}
				bzero(buf, res);
			}
		}
	}
}
