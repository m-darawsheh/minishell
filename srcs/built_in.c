/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_in.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdarawsh <mdarawsh@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/09 13:07:20 by hassende          #+#    #+#             */
/*   Updated: 2025/04/12 16:58:34 by mdarawsh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_builtin(t_cmd *cmd)
{
	if (cmd->cmd_split[0] == NULL)
		return (0);
	if (!ft_strncmp(cmd->cmd_split[0], "echo", 4))
		return (1);
	if (!ft_strncmp(cmd->cmd_split[0], "pwd", 3))
		return (1);
	if (!ft_strncmp(cmd->cmd_split[0], "export", 6))
		return (1);
	if (!ft_strncmp(cmd->cmd_split[0], "unset", 5))
		return (1);
	if (!ft_strncmp(cmd->cmd_split[0], "env", 3))
		return (1);
	if (!ft_strncmp(cmd->cmd_split[0], "exit", 4))
		return (1);
	return (0);
}





// if (tokens[j]->type != TOKEN_WORD)
// {
// 	j++;
// 	continue;
// }
// else
// {
// 	cmd[i]->cmd_split = ft_strdup(tokens[j]->value);
// 	if (!cmd[i]->cmd_split)
// 		exit_error("Malloc failed");
// 	if (cmd[i]->cmd_split[0] == NULL)
// 		return;
// 	j++;
// }
