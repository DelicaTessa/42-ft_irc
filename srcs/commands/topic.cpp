/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   topic.cpp                                          :+:    :+:            */
/*                                                     +:+                    */
/*   By: tessaclement <tessaclement@student.coda      +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/08/24 09:32:18 by tessaclemen   #+#    #+#                 */
/*   Updated: 2022/08/24 11:14:49 by tessaclemen   ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../../headers/Command.hpp"

void Command::topic(std::vector<std::string> cmds, Client & client)
{
    if (!client.getRegistered())
    {
        sendMessage(client, "451", cmds[1], ERR_NOTREGISTERED);
        return;
    }
    if (cmds.size() < 2)
    {
        sendMessage(client, "461", cmds[0], ERR_NEEDMOREPARAMS);
        return;
    }
    std::map<std::string, Channel>::iterator	itMap;
    if ((itMap = chanMap.find(cmds[1])) == chanMap.end())
    {
        sendMessage(client, "403", cmds[1], ERR_NOSUCHCHANNEL);
        return;
    }
    else
    {        
        if (client.isInChan(cmds[1]) == true)
        {
            if (itMap->second.isChanOp(client) == true || itMap->second.getEnableTopic() == true)
            {
                if (cmds.size() > 2)
                {
                    itMap->second.setTopic(cmds[2], client);
                    sendConfirm(client, cmds[0] + " " + cmds[1] + " " + cmds[2], "");
                    return;
                }
                else if (cmds.size() == 2 && itMap->second.getTopic().length() == 0)
                {
                    sendMessage(client, "331", cmds[1], RPL_NOTOPIC);
                    return;
                }
                else if (cmds.size() == 2)
                {
                    sendConfirm(client, "332 " + client.getNick() + " " + cmds[1] + " " + itMap->second.getTopic(), "");
                    sendConfirm(client, "333 " + client.getNick() + " " + cmds[1] + " " + itMap->second.getTopicAuthor(), "");
                    return;
                }
            }
            else
            {
                if (cmds.size() == 2)
                {
                    sendConfirm(client, "332 " + client.getNick() + " " + cmds[1] + " " + itMap->second.getTopic(), "");
                    sendConfirm(client, "333 " + client.getNick() + " " + cmds[1] + " " + itMap->second.getTopicAuthor(), "");
                    return;
                }
                else
                {
                    sendMessage(client, "482", cmds[1], ERR_CHANOPRIVSNEEDED);
                    return;
                }
            }
        }
        else   
        {
            sendMessage(client, "442", cmds[1], ERR_NOTONCHANNEL);
            return;
        }
    }
}
