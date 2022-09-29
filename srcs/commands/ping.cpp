/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ping.cpp                                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: tessaclement <tessaclement@student.coda      +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/08/24 09:31:22 by tessaclemen   #+#    #+#                 */
/*   Updated: 2022/08/24 15:06:26 by tessaclemen   ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../../headers/Command.hpp"

void	Command::ping(std::vector<std::string> cmds, Client & client)
{
    if (cmds.size() == 1)
    {
        sendMessage(client, "461", cmds[0], ERR_NEEDMOREPARAMS);
        return;
    }
    sendSpecConfirm(client, IP, "PONG", cmds[1]);
    return;
}
