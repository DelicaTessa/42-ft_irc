/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   list.cpp                                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: tessaclement <tessaclement@student.coda      +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/08/24 09:21:24 by tessaclemen   #+#    #+#                 */
/*   Updated: 2022/08/24 11:12:12 by tessaclemen   ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../../headers/Command.hpp"

void Command::list(std::vector<std::string> cmds, Client & client)
{
    if (!client.getRegistered())
    {
        sendMessage(client, "451", "", ERR_NOTREGISTERED);
		return ;
	}
    std::map<std::string, Channel>::iterator itMap;
    std::list<Client>::iterator	itClient;
    sendMessage(client, "321", "", "Channel :Users  Name");
    if (cmds.size() == 1)
    {
        itMap = chanMap.begin();
        std::string message;
        for (; itMap != chanMap.end(); itMap++)
        {
            std::string channel(itMap->first);
            itClient = clients.begin();
            if (!message.empty())
                message = "";
            for(; itClient != clients.end(); itClient++)
            {
                if (itClient->isInChan(channel) == true)
                    message.append(itMap->second.getTopic());
            }
            if (message[0] == ':')
                message.erase(0,1);
            sendMessage(client, "322", channel + " :", message);
        }
    }
    else if (cmds.size() > 1)
    {
        std::vector<std::string>	keys;
        std::vector<std::string>::iterator	iChan;
        std::string message;
        keys = ft_split(cmds[1], ",");
        iChan = keys.begin();
        for (; iChan != keys.end(); iChan++)
        {
            if (!message.empty())
                message = "";
            itMap = chanMap.begin();
            for (; itMap != chanMap.end(); itMap++)
            {
                if (itMap->first == *iChan)
                {
                    itClient = clients.begin();
                    for (; itClient != clients.end(); itClient++)
                    {
                        if (itClient->isInChan(itMap->first) == true)
                            message.append(itMap->second.getTopic());
                    }
                    if (message[0] == ':')
                        message.erase(0,1);
                    sendMessage(client, "322", itMap->first + " :", message);
                }
            }
        }
    }
    if (cmds.size() == 1)
        sendMessage(client, "323", "", ":End of /NAMES list.");
    else    
        sendMessage(client, "323", cmds[1] +  " :End of", "/NAMES list.");
    return;
}
