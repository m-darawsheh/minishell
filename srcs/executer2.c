/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executer2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hassende <hassende@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/28 17:16:08 by hassende          #+#    #+#             */
/*   Updated: 2025/05/09 17:25:37 by hassende         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	init_execution(t_pipe_data *pipe_data, int *got_forked)
{
	pipe_data->pipe_fd[0] = -1;
	pipe_data->pipe_fd[1] = -1;
	pipe_data->prev_pipe[0] = -1;
	pipe_data->prev_pipe[1] = -1;
	*got_forked = 0;
	pipe_data->got_forked = got_forked;
}

void	handle_io_redirect(t_cmd *cmd, int *stdin_backup, int *stdout_backup)
{
	*stdout_backup = dup(STDOUT_FILENO);
	*stdin_backup = dup(STDIN_FILENO);
	setup_io_redirections(cmd);
}

void	handle_unquoted_spaces(t_cmd *cmd)
{
	char	**new_args;

	new_args = ft_split(cmd->cmd_split[0], ' ');
	if (new_args)
	{
		if (cmd->cmd_split[1])
			append_args(&new_args, cmd);
		free_2d(cmd->cmd_split);
		cmd->cmd_split = new_args;
	}
}

void	handle_non_piped_cmd(t_cmd *cmd,
		int *stdin_backup, int *stdout_backup)
{
	*stdin_backup = -1;
	*stdout_backup = -1;
	if ((cmd->has_appendfile || cmd->has_infile || cmd->has_outfile)
		&& is_builtin(cmd))
		handle_io_redirect(cmd, stdin_backup, stdout_backup);
	if (cmd->cmd_split && cmd->cmd_split[0]
		&& ft_strchr(cmd->cmd_split[0], ' ') && cmd->was_quoted != 1)
		handle_unquoted_spaces(cmd);
}

void	execute_child(t_cmd *cmd,
		int *pipe_fd, int *prev_pipe, int i)
{
	setup_io_redirections_child(cmd, pipe_fd, prev_pipe, i);
	setup_exec_signals();
	execute_builtin_child(cmd, cmd->path);
}
