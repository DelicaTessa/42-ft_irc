/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   join.cpp                                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: tessaclement <tessaclement@student.coda      +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/08/24 09:20:32 by tessaclemen   #+#    #+#                 */
/*   Updated: 2022/08/24 11:11:20 by tessaclemen   ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../../headers/Command.hpp"

void	Command::join(std::vector<std::string> cmds, Client & client)
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
	if (cmds[1] == "0")
	{
		std::vector<std::string>::iterator	it = client.getChannels().begin();
		std::vector<std::string>	partCmd;
		partCmd.push_back("PART ");
		partCmd.push_back("");
		for (;it != client.getChannels().end(); it++)
			partCmd[1].append(*it + ",");
		part(partCmd, client);
		return;
	}
	std::vector<std::string>	chans = ft_split(cmds[1], ",");
	std::vector<std::string>	keys;
	std::vector<std::string>	names;
	names.push_back("names");
	names.push_back(cmds[1]);
	if (cmds.size() > 2)
		keys = ft_split(cmds[2], ",");
	std::vector<std::string>::iterator	it = chans.begin();
	std::vector<std::string>::iterator	itKey = keys.begin();
	std::map<std::string, Channel>::iterator	itMap;
	for(; it != chans.end(); it++)
	{
		if (it->length() > 50 || (*it)[0] != '#')
			sendMessage(client, "403", cmds[0], *it + ERR_NOSUCHCHANNEL);
		else if ((itMap = chanMap.find(*it)) == chanMap.end())
		{
			itMap =	chanMap.insert(std::pair<std::string, Channel>(*it, Channel())).first;
			itMap->second.addChanOp(&client);
			if (itKey != keys.end())
				itMap->second.setPassword(*itKey);
			client.getChannels().push_back(*it);
			sendConfirm(client, cmds[0] + " " + *it, "");
			sendMessage(client, "324", cmds[1], usedMods(cmds[1]));
			Command::names(names, client);
			itMap->second.sendConfirmChan(client, cmds[0] + " " + *it, "");
		}
		else
		{
			if (!client.isInChan(*it))
			{
				if (!itMap->second.getPassword().empty() && (itKey == keys.end() || itMap->second.getPassword() != *itKey))
				{
					sendMessage(client, "475", *it + ERR_BADCHANNELKEY, "");
				}
				else if (itMap->second.getInvite() && !itMap->second.isInvited(client))
				{
					sendMessage(client, "473", *it + ERR_INVITEONLYCHAN, "");
				}
				else
				{
					itMap->second.addClient(&client);
					client.getChannels().push_back(*it);
					sendConfirm(client, cmds[0] + " " + *it, "");
					sendMessage(client, "324", cmds[1], usedMods(cmds[1]));
					Command::names(names, client);
					if (!itMap->second.getTopic().empty())
					{
						sendMessage(client, "332", *it, itMap->second.getTopic());
						sendMessage(client, "333", *it, itMap->second.getTopicAuthor());
					}
					itMap->second.sendConfirmChan(client, cmds[0] + " " + *it, "");
				}
			}
		}
		if (itKey != keys.end())
			itKey++;
	}
}
