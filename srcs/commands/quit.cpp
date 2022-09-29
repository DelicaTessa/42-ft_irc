/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   quit.cpp                                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: tessaclement <tessaclement@student.coda      +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/08/24 09:31:44 by tessaclemen   #+#    #+#                 */
/*   Updated: 2022/08/24 09:48:36 by tessaclemen   ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../../headers/Command.hpp"

void Command::quit(std::vector<std::string> cmds, Client &client)
{
	if (client.getRegistered())
	{
		if (cmds.size() > 1 && client.getConnected())
		{
			if (cmds[1][0] == ':')
				cmds[1].erase(0, 1);
			sendConfirm(client, cmds[0], cmds[1]);
		}
		else if (client.getConnected())
			sendConfirm(client, cmds[0], client.getNick());
		std::vector<std::string>::iterator it = client.getChannels().begin();
		std::map<std::string, Channel>::iterator	itMap;
		for (; it != client.getChannels().end(); it++)
		{
			itMap = chanMap.find(*it);
			itMap->second.deleteClient(&client);
			if(itMap->second.chanEmpty())
				chanMap.erase(itMap);
		}
		std::list<Client>::iterator itClient = clients.begin();
		for (; itClient != clients.end(); itClient++)
		{
			if (&(*itClient) != &client)
			{
				for (it = client.getChannels().begin(); it != client.getChannels().end(); it++)
				{
					if (itClient->isInChan(*it))
					{
						if (cmds.size() > 1)
						{
							if (cmds[1][0] == ':')
								cmds[1].erase(0, 1);
							sendConfirmTo(*itClient, client, cmds[0], cmds[1]);
						}
						else
							sendConfirmTo(*itClient, client, cmds[0], client.getNick());
						break;
					}
				}
			}
		}
	}
	if (client.getConnected())
		closeLink(client, "Closing Link", client.getIP() + " (Client Quit)");
	if (VERBOSE)
		std::cout << UNDER + "Host disconnected, socket fd : " << client.getSocket() << ", IP : " << client.getIP() << ", port : " << client.getPort() << RESET << std::endl;
	close(client.getSocket());
	client.setbeDeleted(true);
}
