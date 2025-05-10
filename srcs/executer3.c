/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executer3.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hassende <hassende@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/28 17:18:05 by hassende          #+#    #+#             */
/*   Updated: 2025/05/10 13:32:03 by hassende         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	process_non_pipe_cmd(t_cmd *cmd, int *stdin_backup, int *stdout_backup)
{
	handle_non_piped_cmd(cmd, stdin_backup, stdout_backup);
	if (is_builtin(cmd))
		execute_builtin(cmd, cmd->path, *stdin_backup, *stdout_backup);
}

void	process_command(t_cmd **cmd, int i, t_pipe_data *pipe_data)
{
	int	is_child;
	int	stdin_backup;
	int	stdout_backup;

	cmd[i]->main_cmd = cmd;
	is_child = cmd[i]->has_pipe || (i > 0 && cmd[i - 1]->has_pipe);
	if (cmd[i]->skip_cmd)
		return (handle_skip_piped_cmd(cmd[i], pipe_data));
	if (!is_child)
	{
		process_non_pipe_cmd(cmd[i], &stdin_backup, &stdout_backup);
		if (is_builtin(cmd[i]))
			return ;
	}
	*(pipe_data->got_forked) = 1;
	handle_piping(cmd[i], pipe_data->pipe_fd);
	cmd[i]->pid = fork();
	if (cmd[i]->pid == 0)
		execute_child(cmd[i], pipe_data->pipe_fd, pipe_data->prev_pipe, i);
	handle_pipes(pipe_data->pipe_fd, pipe_data->prev_pipe, cmd, i);
}

void	close_all_pipes(t_pipe_data *pipe_data)
{
	if (pipe_data->prev_pipe[0] != -1)
	{
		close(pipe_data->prev_pipe[0]);
		pipe_data->prev_pipe[0] = -1;
	}
	if (pipe_data->pipe_fd[0] != -1)
	{
		close(pipe_data->pipe_fd[0]);
		pipe_data->pipe_fd[0] = -1;
	}
	if (pipe_data->pipe_fd[1] != -1)
	{
		close(pipe_data->pipe_fd[1]);
		pipe_data->pipe_fd[1] = -1;
	}
	if (pipe_data->prev_pipe[1] != -1)
	{
		close(pipe_data->prev_pipe[1]);
		pipe_data->prev_pipe[1] = -1;
	}
}

void	execute_command(t_cmd **cmd, t_cmd_path *path)
{
	int			i;
	int			got_forked;
	t_pipe_data	pipe_data;

	i = -1;
	init_execution(&pipe_data, &got_forked);
	while (cmd[++i])
		process_command(cmd, i, &pipe_data);
	close_all_pipes(&pipe_data);
	setup_interactive_signals();
	if (got_forked)
		wait_for_children(path, cmd);
}

void	execute_echo_exit_cd(t_cmd *cmd, t_cmd_path *path)
{
	if (!ft_strcmp(cmd->cmd_split[0], "echo"))
	{
		path->exit_status = 0;
		do_echo(cmd);
	}
	else if (!ft_strcmp(cmd->cmd_split[0], "exit"))
		path->exit_status = do_exit(cmd);
	else if (!ft_strcmp(cmd->cmd_split[0], "cd"))
		path->exit_status = do_cd(cmd, path);
}
