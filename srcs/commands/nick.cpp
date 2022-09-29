/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   nick.cpp                                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: tessaclement <tessaclement@student.coda      +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/08/24 09:30:31 by tessaclemen   #+#    #+#                 */
/*   Updated: 2022/08/24 11:16:35 by tessaclemen   ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../../headers/Command.hpp"

void	Command::nick(std::vector<std::string> cmds, Client & client)
{
	if (cmds.size() == 1)
	{
		sendMessage(client, "431", "", ERR_NONICKNAMEGIVEN);
		return ;
	}
	if (cmds[1].length() > 8)
		cmds[1].resize(9);
	std::list<Client>::iterator	it = clients.begin();
	for(; it != clients.end(); it++)
		if (cmds[1] == it->getNick() && &(*it) != &client)
		{
			sendMessage(client, "433", cmds[1], ERR_NICKNAMEINUSE);
			return ;
		}
	if ((!isalpha(cmds[1][0]) && !isSpecialChar(cmds[1][0])))
	{
		sendMessage(client, "432", cmds[1], ERR_ERRONEUSNICKNAME);
		return;
	}
	for (size_t i = 1; i < cmds[1].length(); i++)
	{
		if (!isalpha(cmds[1][i]) && !isSpecialChar(cmds[1][i]) && !isdigit(cmds[1][i]))
		{
			sendMessage(client, "432", cmds[1], ERR_ERRONEUSNICKNAME);
			return;
		}
	}
	if (client.getNick() != cmds[1])
	{
		if (client.getRegistered())
			sendConfirm(client, cmds[0], cmds[1]);
		client.setNick(cmds[1]);
	}
	if (!client.getRegistered())
	{
		client.setNicked(true);
		registerClient(client);
	}
}
