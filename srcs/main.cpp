/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   main.cpp                                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: tessaclement <tessaclement@student.coda      +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/08/24 09:21:31 by tessaclemen   #+#    #+#                 */
/*   Updated: 2022/08/24 11:08:07 by tessaclemen   ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/Server.hpp"

void	ft_irc(Server & server)
{
	try
	{
		server.initialize();
		server.launch();
	}
	catch (std::exception & e)
	{
		std::cerr << RED + "exception caught: " << e.what() + RESET << std::endl;
	}
}

int	main(int ac, char **av)
{
	int	port = 0;

	if (ac == 3)
	{
		port = atoi(av[1]);
		if (port < 1024 || port > 65535)
		{
			std::cout << RED + "Error Port : must be an int >= 1024 and =< 65535" + RESET << std::endl;
			return (1);
		}
		Server	server(port, av[2]);
		ft_irc(server);
	}
	else
	{
		std::cout << YELLOW + "right syntax is ./ircserv <port> <password>" + RESET << std::endl;
		return (1);
	}
	return (0);
}
