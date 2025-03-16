/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hassende <hassende@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/25 16:19:37 by hassende          #+#    #+#             */
/*   Updated: 2025/03/16 14:57:00 by hassende         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	exit_error(char *str)
{
	ft_putendl_fd(str, 2);
	exit(1);
}

void	*print_and_null(char *str)
{
	ft_putendl_fd(str, 2);
	return (NULL);
}

void	free_cmds(t_cmd **cmd)
{
	int	i;

	i = -1;
	while (cmd[++i])
	{
		free_2d(cmd[i]->cmd_split);
		free(cmd[i]->infile);
		free(cmd[i]->outfile);
		free(cmd[i]->cmd);
		free(cmd[i]->cmd_path);
		free(cmd[i]->delimiter);
		free(cmd[i]);
	}
	free(cmd);
}

// ! removed for now, will be added later
// void	free_all(t_cmd_path *cmd)
// {
// 	free_2d(cmd->cmd_split);
// 	free_2d(cmd->path);
// 	rl_clear_history();
// 	if (cmd->cmd)
// 		free(cmd->cmd);
// }
