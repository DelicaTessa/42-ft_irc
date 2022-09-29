/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   privmsg.cpp                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: tessaclement <tessaclement@student.coda      +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/08/24 09:31:33 by tessaclemen   #+#    #+#                 */
/*   Updated: 2022/08/24 11:13:56 by tessaclemen   ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../../headers/Command.hpp"

void	Command::privatmsg(std::vector<std::string> cmds, Client & client)
{
    if(!client.getRegistered())
    {
        sendMessage(client, "451","", ERR_NOTREGISTERED);
        return;
    }
    if(cmds.size() == 1)
    {
        sendMessage(client, "411", "", ERR_NORECIPIENT);
        return ;
    }
    if(cmds.size() == 2)
    {
        sendMessage(client, "412", "", ERR_NOTEXTTOSEND);
        return;
    }
    if (cmds[1][0] == '#')
    {
        std::map<std::string, Channel>::iterator	itMap;
        if ((itMap = chanMap.find(cmds[1])) != chanMap.end())
        {
            if (!client.isInChan(itMap->first))
            {
                sendMessage(client, "404", cmds[1], ERR_CANNOTSENDTOCHAN);
                return;   
            }
            if (cmds[2][0] == ':')
                cmds[2].erase(0, 1);
            itMap->second.sendConfirmChan(client, cmds[0] + " " + cmds[1], cmds[2]);
            return;
        }
        else
        {
            sendMessage(client, "401", cmds[1], ERR_NOSUCHNICK);
            return;
        }
    }
    else
    {
        std::list<Client>::iterator	it = clients.begin();
        for (; it != clients.end(); it++)
            if (cmds[1] == it->getNick())
                break;
        if (it == clients.end())
        {
            sendMessage(client, "401", "", ERR_NOSUCHNICK);
            return;
        }
        else
        {
            if (it->getAway().first == true)
                sendMessage(client, "301", it->getNick() ,it->getAway().second);
            if (cmds[2][0] == ':')
                cmds[2].erase(0, 1);
            sendConfirmTo(*it, client, cmds[0] + " " + cmds[1], cmds[2]);
            return;
        }
    }
}
