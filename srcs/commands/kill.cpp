/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   kill.cpp                                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: tessaclement <tessaclement@student.coda      +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/08/24 09:21:09 by tessaclemen   #+#    #+#                 */
/*   Updated: 2022/08/24 11:12:14 by tessaclemen   ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../../headers/Command.hpp"

void Command::kill(std::vector<std::string> cmds, Client &client)
{
	if (!client.getRegistered())
	{
		sendMessage(client, "451", "", ERR_NOTREGISTERED);
		return;
	}
	if (!client.getOperator())
	{
		sendMessage(client, "481", "", ERR_NOPRIVILEGES);
		return;
	}
	if (cmds.size() < 2)
	{
		sendMessage(client, "461", cmds[0], ERR_NEEDMOREPARAMS);
		return;
	}
	std::list<Client>::iterator it;
	for (it = clients.begin(); it != clients.end(); it++)
	{
		if (cmds[1] == it->getNick())
		{
			if (cmds.size() > 2)
			{
				if (cmds[2][0] == ':')
					cmds[2].erase(0, 1);
				sendConfirmTo(*it, client, cmds[0], cmds[2]);
			}
			else
				sendConfirmTo(*it, client, cmds[0], it->getNick());
			std::vector<std::string> quitCmd(1, "QUIT");
			quitCmd.push_back(":" + it->getNick() + " has been killed by an operator");
			it->setConnected(false);
			quit(quitCmd, *it);
			return;
		}
	}
	sendMessage(client, "401", cmds[1], ERR_NOSUCHNICK);
}
