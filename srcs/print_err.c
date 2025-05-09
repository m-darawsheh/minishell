/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_err.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hassende <hassende@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/16 19:02:08 by hassende          #+#    #+#             */
/*   Updated: 2025/05/08 17:26:39 by hassende         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_path(t_cmd_path *path)
{
	if (path->path)
		free_2d(path->path);
	if (path->envp)
		free_2d(path->envp);
}

void	free_single_cmd(t_cmd *cmd)
{
	if (!cmd)
		return ;
	if (cmd->cmd_split)
		free_2d(cmd->cmd_split);
	if (cmd->infile)
		free(cmd->infile);
	if (cmd->outfile)
		free(cmd->outfile);
	if (cmd->cmd)
		free(cmd->cmd);
	if (cmd->delimiter)
		free(cmd->delimiter);
	if (cmd->main_cmd)
		free(cmd->main_cmd);
	free(cmd);
}

void	print_not_found(t_cmd *cmd, t_cmd_path *path)
{
	char	*temp;

	temp = ft_strjoin(cmd->cmd_split[0], " : command not found\n");
	write(2, temp, ft_strlen(temp));
	free(temp);
	free_cmds(cmd->main_cmd, 1);
	free_path(path);
	exit(127);
}

void	print_dir_error(t_cmd *cmd)
{
	char	*temp;

	temp = ft_strjoin(cmd->cmd_split[0], " : Is a directory\n");
	write(2, temp, ft_strlen(temp));
	free(temp);
	free_path(cmd->path);
	free_cmds(cmd->main_cmd, 1);
	exit(126);
}
