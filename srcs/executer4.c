/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executer4.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hassende <hassende@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/28 17:19:46 by hassende          #+#    #+#             */
/*   Updated: 2025/05/10 13:32:08 by hassende         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	handle_piping(t_cmd *cmd, int *pipe_fd)
{
	if (cmd->has_pipe)
		if (pipe(pipe_fd) == -1)
			exit_error("Pipe failed");
}

void	handle_skip_piped_cmd(t_cmd *cmd, t_pipe_data *pipe_data)
{
	cmd->pid = -1;
	if (cmd->has_pipe)
	{
		handle_piping(cmd, pipe_data->pipe_fd);
		close(pipe_data->pipe_fd[1]);
		pipe_data->prev_pipe[0] = pipe_data->pipe_fd[0];
		pipe_data->prev_pipe[1] = -1;
	}
}

void	append_args(char ***new_args, t_cmd *cmd)
{
	int	i;
	int	j;

	i = 0;
	while ((*new_args)[i])
		i++;
	j = 1;
	while (cmd->cmd_split[j])
		j++;
	*new_args = realloc_2d(*new_args, i, i + j);
	if (!*new_args)
		return ;
	j = 1;
	while (cmd->cmd_split[j])
	{
		(*new_args)[i] = ft_strdup(cmd->cmd_split[j]);
		if (!(*new_args)[i])
			return ;
		i++;
		j++;
	}
	(*new_args)[i] = NULL;
}

void	restore_io(int stdin_backup, int stdout_backup)
{
	if (stdin_backup != -1)
	{
		dup2(stdin_backup, STDIN_FILENO);
		close(stdin_backup);
	}
	if (stdout_backup != -1)
	{
		dup2(stdout_backup, STDOUT_FILENO);
		close(stdout_backup);
	}
}
