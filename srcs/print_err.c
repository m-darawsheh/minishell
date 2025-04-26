/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_err.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdarawsh <mdarawsh@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/16 19:02:08 by hassende          #+#    #+#             */
/*   Updated: 2025/04/26 17:13:23 by mdarawsh         ###   ########.fr       */
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
	printf("%s", cmd->cmd_split[0]);
	printf(":  command not found\n");
	if (cmd->main_cmd)
		free_cmds(cmd->main_cmd, 1);
	else
		free_single_cmd(cmd);
	free_path(path);
	exit(127);
}

void	print_dir_error(char *cmd)
{
	ft_putstr_fd("minishell: ", 2);
	ft_putstr_fd(cmd, 2);
	ft_putstr_fd(": is a directory\n", 2);
	exit(126);
}
