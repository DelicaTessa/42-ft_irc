/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   pass.cpp                                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: tessaclement <tessaclement@student.coda      +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/08/24 09:31:13 by tessaclemen   #+#    #+#                 */
/*   Updated: 2022/08/24 11:13:35 by tessaclemen   ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../../headers/Command.hpp"

void	Command::pass(std::vector<std::string> cmds, Client & client)
{
	if (cmds.size() == 1)
	{
		sendMessage(client, "461", "", ERR_NEEDMOREPARAMS);
		return ;
	}
	if (client.getRegistered())
	{
		sendMessage(client, "462", "", ERR_ALREADYREGISTRED);
		return;
	}
	client.setPass(cmds[1]);
}
