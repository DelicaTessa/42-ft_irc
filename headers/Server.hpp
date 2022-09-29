/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   Server.hpp                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: tessaclement <tessaclement@student.coda      +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/08/24 08:58:57 by tessaclemen   #+#    #+#                 */
/*   Updated: 2022/08/26 10:24:33 by tessaclemen   ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
#define SERVER_HPP

# include "Client.hpp"
# include "Command.hpp"
# include "messages.hpp"
# include <netinet/in.h>
# include <errno.h>
# include <unistd.h>
# include <iostream>
# include <list>
# include <vector>
# include <string>
# include <stdexcept>
# include <sys/select.h>
# include <fcntl.h>
# include <sys/socket.h>
# include <arpa/inet.h>
# include <netdb.h>

class	Server
{
	public:
			Server(int port, std::string const & pass);
			Server();
			~Server();

			void	findHostname();
			void	initialize();
			void	launch();

	private:
			int	port;
			std::string	pass;
			std::string	IP;
			std::list<Client>	clients;
			Command	command; 

			int	head_socket;
			struct sockaddr_in address;
			int	addrlen;
			fd_set	readfds;
};

#endif
