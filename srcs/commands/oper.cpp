/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   oper.cpp                                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: tessaclement <tessaclement@student.coda      +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/08/24 09:30:56 by tessaclemen   #+#    #+#                 */
/*   Updated: 2022/08/24 11:13:17 by tessaclemen   ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../../headers/Command.hpp"

void Command::oper(std::vector<std::string> cmds, Client & client)
{
	if (!client.getRegistered())
	{
        sendMessage(client, "451", "", ERR_NOTREGISTERED);
        return;
	}
	size_t	a = 3;
	if (password.empty())
		a = 2;
    if (cmds.size() < a)
    {
        sendMessage(client, "461", cmds[0], ERR_NEEDMOREPARAMS);
        return;
    }
	std::list<Client>::iterator	it = clients.begin();
	for (; it != clients.end(); it++)
		if (cmds[1] == it->getNick())
			break;
    if (it == clients.end() || !it->getRegistered())
    {
        sendMessage(client, "491", "", ERR_NOOPERHOST);
        return;
    }
    if (!password.empty() && cmds[2] != password)
    {
        sendMessage(client, "464", "", ERR_PASSWDMISMATCH);
        return;
    }
	if (!it->getOperator())
	{
    	it->setOperator(true);
    	sendMessage(*it, "381", "", RPL_YOUREOPER);
	}
    return;
}
