/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdarawsh <mdarawsh@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/25 16:19:37 by hassende          #+#    #+#             */
/*   Updated: 2025/04/16 14:54:08 by mdarawsh         ###   ########.fr       */
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
	if (!cmd)
		return ;
	while (cmd[++i])
	{
		if (cmd[i]-> cmd_split)
			free_2d(cmd[i]->cmd_split);
		if (cmd[i]->infile)
			free(cmd[i]->infile);
		if (cmd[i]->outfile)
			free(cmd[i]->outfile);
		if (cmd[i]->cmd)
			free(cmd[i]->cmd);
		if (cmd[i]->cmd_path)
			free(cmd[i]->cmd_path);
		if (cmd[i]->delimiter)
			free(cmd[i]->delimiter);
		// if (cmd[i])
		// 	free(cmd[i]);
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
