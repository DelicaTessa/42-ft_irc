/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   mode.cpp                                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: tessaclement <tessaclement@student.coda      +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/08/24 09:29:50 by tessaclemen   #+#    #+#                 */
/*   Updated: 2022/08/24 15:02:27 by tessaclemen   ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../../headers/Command.hpp"

void Command::mode(std::vector<std::string> cmds, Client & client)
{
    int error_nbr = 0;
    int sign = '+';
    if (!client.getRegistered())
    {
        sendMessage(client, "451", "", ERR_NOTREGISTERED);
        return;
    }
    if (cmds.size() == 1)
    {
        sendMessage(client, "461", cmds[0], ERR_NEEDMOREPARAMS);
        return;
    }
    if (cmds.size() == 2)
    {
        std::time_t result = std::time(nullptr);
        std::map<std::string, Channel>::iterator    itMap = chanMap.begin();    
        std::list<Client>::iterator	it = clients.begin();
        if (cmds[1][0] == '#')
        {
            if ((itMap = chanMap.find(cmds[1])) == chanMap.end())
                sendMessage(client, "403", cmds[1], ERR_NOSUCHCHANNEL);
            else
            {
                sendMessage(client, "324", cmds[1], usedMods(cmds[1]));
                sendMessage(client, "333", cmds[1], std::to_string((int)result));
            }
        }
        else
        {
            for(; it != clients.end(); it++)
            {
                if (it->getNick() == cmds[1])
                {
                    if (it->getOperator() == true)
                        sendMessage(client, "324", "", "+o");
                    else
                        sendMessage(client, "324", "", "+");
                    break;
                }
            }
            if (it == clients.end())
                sendMessage(client, "403", cmds[1], ERR_NOSUCHCHANNEL);
        }
    }
    else if (cmds[1][0] != '#')
    {
        if (cmds[1] != client.getNick())
        {
            sendMessage(client, "502", "", ERR_USERSDONTMATCH);
            return;
        }
        if (client.getNick() == cmds[1] || cmds[2][0] == 'b')
            return;
        for (size_t i = 0;i < cmds[2].length(); i++)
        {
            if (cmds[2][i] == '-')
                sign = '-';
            else if (cmds[2][i] == '+')
                sign = '+';
            else if (cmds[2][i] == 'o')
            {
                if (sign == '-')
                {    
                    if(client.getOperator() == true)
                    {
                        client.setOperator(false);
                        sendSpecConfirm(client , client.getNick(), cmds[0], cmds[2]);
                    }
                }
            }
            else if (error_nbr == 0)
            {
                sendMessage(client, "501", "", ERR_UMODEUNKNOWNFLAG);
                error_nbr++;
            }
        }
    }
    else if (cmds[1][0] == '#')
    {
		std::string	arg("+");
		std::string	opt;
		bool	okay = false;
        std::map<std::string, Channel>::iterator	itMap;
        if ((itMap = chanMap.find(cmds[1])) != chanMap.end())
        {
            if (client.isInChan(cmds[1]) == true)
            {
                if(itMap->second.isChanOp(client) == true)
                {
                    for (size_t i = 0;i < cmds[2].length(); i++)
					{
                        if (cmds[2][0] == 'b' && cmds[2].length() == 1)
                            return;
						if (cmds[2][i] != '-' && cmds[2][i] != '+' && cmds[2][i] != 'o' && cmds[2][i] != 'i' && cmds[2][i] != 't' && cmds[2][i] != 'k')
                        {
							std::string s;
							s.push_back(cmds[2][i]);
                            sendMessage(client, "472", s, ERR_UMODEUNKNOWNFLAG + itMap->first);
							return ;
                        }
					}
                    for (size_t i = 0;i < cmds[2].length(); i++)
                    {    
                        if (cmds[2][i] == '-' && sign != '-')
						{
                            sign = '-';
							arg.append("-");
						}
                        else if (cmds[2][i] == '+' && sign != '+')
						{
                            sign = '+';
							arg.append("+");
						}
                        else if (cmds[2][i] == 'b')
                            break;
                        else if (cmds[2][i] == 'o')
                        {
                            if (cmds.size() < 4)
                            {
                                sendMessage(client, "461", cmds[0], ERR_NEEDMOREPARAMS);
								continue ;
                            }
                            std::list<Client>::iterator	it = clients.begin();
                            for (; it != clients.end(); it++)
                                if (cmds[3] == it->getNick())
                                    break;
                            if (it == clients.end())
                            {
                                sendMessage(client, "401", "", ERR_NOSUCHNICK);
								continue ;
                            }
                            if (it->isInChan(cmds[1]) == true)
                            {                                
                                if (sign == '-')
                                {
                                    if (itMap->second.isChanOp(*it) == true)
                                    {
                                        itMap->second.deleteChanOp(&(*it));
										itMap->second.addClient(&(*it));
										arg.append("o");
										okay = true;
                                    }
                                }
                                else if (sign == '+')
                                {
                                    if (itMap->second.isChanOp(*it) == false)
                                    {
                                        itMap->second.addChanOp(&(*it));                                
										itMap->second.deleteClient(&(*it));
										arg.append("o");
										okay = true;
                                    }
                                }
                            }
                            else
                            {
                                sendMessage(client, "442", client.getNick() + " " +  cmds[3] + " " + cmds[1], ERR_USERNOTINCHANNEL);
								continue ;
                            }
                        }
                        else if (cmds[2][i] == 'i')
                        {
                            if (sign == '-')
                            {
                                if (itMap->second.getInvite() == true)
                                {
                                    itMap->second.setInvite(false);
									arg.append("i");
									okay = true;
                                }
                            }
                            if (sign == '+')
                            {                                
                                if (itMap->second.getInvite() == false)
                                {
                                    itMap->second.setInvite(true);
									arg.append("i");
									okay = true;
                                }
                            }
                        }
                        else if (cmds[2][i] == 't')
                        {
                            if (sign == '-')
                            {
                                if (itMap->second.getEnableTopic() == true)
                                {
                                    itMap->second.enableTopic(false);
									arg.append("t");
									okay = true;
                                }
                            }
                            else if (sign == '+')
                            {
                                if (itMap->second.getEnableTopic() == false)
                                {
                                    itMap->second.enableTopic(true);
									arg.append("t");
									okay = true;
                                }
                            }
                        }
                        else if (cmds[2][i] == 'k')
                        {
                            if (sign == '-')
                            {
                                if (cmds.size() == 4 && cmds[3] == itMap->second.getPassword())
                                {
                                    itMap->second.setPassword("");
									opt.assign("*");
									arg.append("k");
									okay = true;
                                }
                            }
                            else if (sign == '+')
                            {
                                if (cmds.size() == 4)
                                {
                                    itMap->second.setPassword(cmds[3]);
									opt.assign(cmds[3]);
									arg.append("k");
									okay = true;
                                }
                            }
                        }
                    }
					if (okay)
					{
						size_t i = 0;
						while(i < arg.length())
						{
							if ((i + 1) != arg.length() && (arg[i + 1] == '-' || arg[i + 1] == '+') && (arg[i] == '-' || arg[i] == '+'))
								arg.erase(i, 1);
							else
								i++;
						}
						for (int i = arg.length() - 1; i != 0; i--)
						{
							if (arg[i] == '-' || arg[i] == '+')
								arg.erase(i, 1);
							else
								break;
						}
						if (opt.empty())
						{
							sendConfirm(client , cmds[0] + " " + cmds[1] + " " + arg, "");
							itMap->second.sendConfirmChan(client, cmds[0] + " " + cmds[1] + " " + arg, "");	
						}
						else
						{
							sendConfirm(client , cmds[0]  + " " + cmds[1] + " " + arg + " " + opt, "");
							itMap->second.sendConfirmChan(client, cmds[0] + " " + cmds[1] + " " + arg + " " + opt, "");	
						}
					}
                }
                else
                {
                    sendMessage(client, "482", cmds[1], ERR_CHANOPRIVSNEEDED);
                    return;
                }
            }
            else if (client.isInChan(cmds[1]) == false)
            {
                sendMessage(client, "482", cmds[1], ERR_CHANOPRIVSNEEDED);
                return;
            }
        }
        else
        {
            sendMessage(client, "482", cmds[1], ERR_CHANOPRIVSNEEDED);
            return;
        }
    }
    return;
}
