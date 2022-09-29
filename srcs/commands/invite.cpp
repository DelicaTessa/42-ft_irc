/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   invite.cpp                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: tessaclement <tessaclement@student.coda      +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/08/24 09:20:11 by tessaclemen   #+#    #+#                 */
/*   Updated: 2022/08/24 11:11:09 by tessaclemen   ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../../headers/Command.hpp"

void    Command::invite(std::vector<std::string> cmds, Client & client)
{
	if (!client.getRegistered())
	{
		sendMessage(client, "451", "", ERR_NOTREGISTERED);
		return ;
	}
	if (cmds.size() < 2)
	{
		sendMessage(client, "461", cmds[0], ERR_NEEDMOREPARAMS);
		return ;
	}
	std::list<Client>::iterator	it = clients.begin();
	for (; it != clients.end(); it++)
		if (it->getNick() == cmds[1])
			break;
	if (it == clients.end())
	{
        sendMessage(client, "401", cmds[1], ERR_NOSUCHNICK);
        return;
	}
	if (!client.isInChan(cmds[2]))
	{
		sendMessage(client, "442", cmds[2], ERR_NOTONCHANNEL);
		return;
	}
	if (!chanMap[cmds[2]].isChanOp(client))
	{
		sendMessage(client, "482", cmds[2] + " ", ERR_CHANOPRIVSNEEDED);
		return;
	}
	if (it->isInChan(cmds[2]))
	{
		sendMessage(client, "443", cmds[1] + " " + cmds[2], ERR_USERONCHANNEL);
		return;
	}
	sendMessage(client, "341", cmds[1] + " " + cmds[2], "");
	sendConfirmTo(*it, client, cmds[0] + " " + it->getNick(), cmds[2]);
	std::map<std::string, Channel>::iterator	itMap;
	if ((itMap = chanMap.find(cmds[2])) != chanMap.end())
	{
		itMap->second.addInvited(&(*it));
		std::cout << itMap->first << std::endl;	
	}
	else
	{
		sendMessage(client, "401", cmds[1], ERR_NOSUCHNICK);
		return;
	}
	if (it->getAway().first)
	{
		sendMessage(client, "301", it->getNick(), it->getAway().second);
		return;
	}
}
