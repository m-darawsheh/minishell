/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executer.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hassende <hassende@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/16 18:42:30 by hassende          #+#    #+#             */
/*   Updated: 2025/05/09 17:36:27 by hassende         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	execute_export_env_pwd_unset(t_cmd *cmd, t_cmd_path *path)
{
	if (!ft_strcmp(cmd->cmd_split[0], "export"))
	{
		path->exit_status = 0;
		export_handle(cmd, path);
	}
	else if (!ft_strcmp(cmd->cmd_split[0], "env"))
	{
		path->exit_status = 0;
		print_env(path);
	}
	else if (!ft_strcmp(cmd->cmd_split[0], "pwd"))
	{
		path->exit_status = 0;
		pwd_handle(path);
	}
	else if (!ft_strcmp(cmd->cmd_split[0], "unset"))
	{
		path->exit_status = 0;
		handle_unset(cmd, path);
	}
}

void	execute_builtin(t_cmd *cmd, t_cmd_path *path,
		int stdin_backup, int stdout_backup)
{
	execute_echo_exit_cd(cmd, path);
	execute_export_env_pwd_unset(cmd, path);
	restore_io(stdin_backup, stdout_backup);
}

void	do_builtin_children(t_cmd *cmd, t_cmd_path *path)
{
	execute_echo_exit_cd(cmd, path);
	execute_export_env_pwd_unset(cmd, path);
}

int	expanded_as_command(t_cmd *cmd)
{
	char	**new_args;

	if (!ft_strchr(cmd->cmd_split[0], ' '))
		return (0);
	new_args = ft_split(cmd->cmd_split[0], ' ');
	if (!new_args)
		return (0);
	if (cmd->cmd_split[1])
		append_args(&new_args, cmd);
	if (!new_args)
		return (0);
	free_2d(cmd->cmd_split);
	cmd->cmd_split = new_args;
	setup_command(cmd, cmd->path);
	return (1);
}

void	no_path_error(t_cmd *cmd, t_cmd_path *path)
{
	char	*temp;

	temp = ft_strjoin(cmd->cmd_split[0], " : No such file or directory\n");
	write(2, temp, ft_strlen(temp));
	free(temp);
	free_cmds(cmd->main_cmd, 1);
	free_path(path);
	exit(127);
}

void	execute_builtin_child(t_cmd *cmd, t_cmd_path *path)
{
	if (cmd->cmd_split && cmd->cmd_split[0])
	{
		if (is_builtin(cmd))
		{
			do_builtin_children(cmd, path);
			free_cmds(cmd->main_cmd, 1);
			free_path(path);
			exit(0);
		}
		if (!expanded_as_command(cmd))
		{
			if (path->path)
				free_2d(path->path);
			path->path = find_path(path->envp);
			if (!path->path)
				no_path_error(cmd, path);
			setup_command(cmd, path);
		}
		execve(cmd->cmd_path, cmd->cmd_split, path->envp);
		free(cmd->cmd_path);
	}
	free_cmds(cmd->main_cmd, 1);
	free_path(path);
	exit(127);
}
