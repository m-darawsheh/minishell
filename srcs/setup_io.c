/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   setup_io.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hassende <hassende@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/16 18:40:15 by hassende          #+#    #+#             */
/*   Updated: 2025/05/10 13:30:43 by hassende         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	setup_io_continued(t_cmd *cmd)
{
	int	fd;

	if (cmd->has_appendfile)
	{
		fd = open(cmd->outfile, O_CREAT | O_WRONLY | O_APPEND, 0644);
		if (fd == -1)
			exit_error("File not found");
		dup2(fd, STDOUT_FILENO);
		close(fd);
	}
	if (cmd->has_heredoc)
	{
		dup2(cmd->heredoc_fd, STDIN_FILENO);
		close(cmd->heredoc_fd);
	}
}

static void	setup_io_child_continued(t_cmd *cmd, int fd)
{
	if (cmd->has_infile)
	{
		fd = open(cmd->infile, O_RDONLY);
		if (fd == -1)
			exit_error("File not found");
		dup2(fd, STDIN_FILENO);
		close(fd);
	}
	if (cmd->has_outfile)
	{
		fd = open(cmd->outfile, O_WRONLY | O_CREAT | O_TRUNC, 0644);
		if (fd == -1)
			exit_error("File not found");
		dup2(fd, STDOUT_FILENO);
		close(fd);
	}
	if (cmd->has_appendfile)
	{
		fd = open(cmd->outfile, O_WRONLY | O_CREAT | O_APPEND, 0644);
		if (fd == -1)
			exit_error("File not found");
		dup2(fd, STDOUT_FILENO);
		close(fd);
	}
}

void	setup_io_redirections(t_cmd *cmd)
{
	int	fd;

	if (cmd->has_infile)
	{
		fd = open(cmd->infile, O_RDONLY);
		if (fd == -1)
			exit_error("File not found");
		dup2(fd, STDIN_FILENO);
		close(fd);
	}
	if (cmd->has_outfile)
	{
		fd = open(cmd->outfile, O_CREAT | O_WRONLY | O_TRUNC, 0644);
		if (fd == -1)
			exit_error("File not found");
		dup2(fd, STDOUT_FILENO);
		close(fd);
	}
	setup_io_continued(cmd);
}

void	setup_io_redirections_child(t_cmd *cmd, int *pipe_fd,
			int *prev_pipe, int i)
{
	int	fd;

	fd = -1;
	if (i > 0 && prev_pipe[0] != -1 && !cmd->has_heredoc && !cmd->has_infile)
		dup2(prev_pipe[0], STDIN_FILENO);
	if (i > 0 && prev_pipe[0] != -1)
	{
		close(prev_pipe[0]);
		if (prev_pipe[1] != -1)
			close(prev_pipe[1]);
	}
	if (cmd->has_heredoc)
	{
		dup2(cmd->heredoc_fd, STDIN_FILENO);
		close(cmd->heredoc_fd);
	}
	clean_main_cmd_fds(cmd);
	if (cmd->has_pipe)
	{
		dup2(pipe_fd[1], STDOUT_FILENO);
		close(pipe_fd[0]);
		close(pipe_fd[1]);
	}
	setup_io_child_continued(cmd, fd);
}

void	handle_pipes(int *pipe_fd, int *prev_pipe, t_cmd **cmd, int i)
{
	if (i > 0 && cmd[i - 1]-> has_pipe)
	{
		close(prev_pipe[0]);
		if (prev_pipe[1] != -1)
			close(prev_pipe[1]);
	}
	if (cmd[i]->has_pipe)
	{
		if (cmd[i + 1] && (cmd[i + 1]->has_infile || cmd[i + 1]->has_heredoc))
		{
			close(pipe_fd[1]);
			prev_pipe[0] = pipe_fd[0];
			prev_pipe[1] = -1;
		}
		else
		{
			prev_pipe[0] = pipe_fd[0];
			prev_pipe[1] = pipe_fd[1];
		}
	}
	else
	{
		prev_pipe[0] = -1;
		prev_pipe[1] = -1;
	}
}
