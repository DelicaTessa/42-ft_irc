/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   away.cpp                                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: tessaclement <tessaclement@student.coda      +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/08/24 09:01:34 by tessaclemen   #+#    #+#                 */
/*   Updated: 2022/08/24 11:10:59 by tessaclemen   ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../../headers/Command.hpp"

void	Command::away(std::vector<std::string> cmds, Client & client)
{
    if (!client.getRegistered())
    {
        sendMessage(client, "451", "", ERR_NOTREGISTERED);
        return;
    }
    if (cmds.size() == 1)
    {
        client.setAway(std::pair<bool, std::string>(false, ""));
        sendMessage(client, "305", "", RPL_UNAWAY);
        return;
    }
    if (cmds.size() > 1)
    {
        client.setAway(std::pair<bool, std::string>(true, cmds[1]));
        sendMessage(client, "306", "", RPL_NOWAWAY);
        return;
    }
}
