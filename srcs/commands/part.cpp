/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   part.cpp                                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: tessaclement <tessaclement@student.coda      +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/08/24 09:31:05 by tessaclemen   #+#    #+#                 */
/*   Updated: 2022/08/26 12:29:12 by tessaclemen   ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../../headers/Command.hpp"

void Command::part(std::vector<std::string> cmds, Client &client)
{
	if (!client.getRegistered())
	{
		sendMessage(client, "451", "", ERR_NOTREGISTERED);
		return;
	}
	if (cmds.size() < 2)
	{
		sendMessage(client, "461", cmds[0], ERR_NEEDMOREPARAMS);
		return;
	}
	std::vector<std::string> chans = ft_split(cmds[1], ",");
	std::vector<std::string>::iterator it = chans.begin();
	std::map<std::string, Channel>::iterator itMap;
	for (; it != chans.end(); it++)
	{
		if ((itMap = chanMap.find(*it)) == chanMap.end())
			sendMessage(client, "403", cmds[0], *it + ERR_NOSUCHCHANNEL);
		else
		{
			if (client.isInChan(*it))
			{
				sendConfirm(client, cmds[0] + " " + *it, "");
				itMap->second.sendConfirmChan(client, cmds[0] + " " + *it, "");
				itMap->second.deleteClient(&client);
				client.deleteChan(*it);
				if (itMap->second.chanEmpty())
					chanMap.erase(itMap);
			}
			else
				sendMessage(client, "442", cmds[0], *it + ERR_NOTONCHANNEL);
		}
	}
}
