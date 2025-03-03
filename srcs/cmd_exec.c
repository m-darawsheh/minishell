/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_exec.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hassende <hassende@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/03 12:48:35 by hassende          #+#    #+#             */
/*   Updated: 2025/03/03 15:53:02 by hassende         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	exec_cmd(char *line_read, t_cmd_path *path)
{
	t_cmd	**cmd;
	int		i;
	int		pipe_fd[2];
	int		prev_pipe[2];
	pid_t	pid;

	i = -1;
	prev_pipe[0] = -1;
	prev_pipe[1] = -1;
	cmd = t_cmd_malloc(line_read);
	if (!cmd)
		exit_error("Malloc failed");
	init_cmds(cmd, line_read);
	while (cmd[++i])
	{
		if (cmd[i]->has_pipe)
		{
			if (pipe(pipe_fd) == -1)
				exit_error("Pipe failed");
		}
		pid = fork();
		if (pid == 0)
		{
			setup_command(cmd[i], path);
			// if this command got a pipe, output to it
			if (cmd[i]->has_pipe)
			{
				printf("I got here !\n");
				dup2(pipe_fd[1], 1);
				printf("I got here !\n");
				close(pipe_fd[0]);
				close(pipe_fd[1]);
			}
			// if this command reads from a file, input from it
			if (cmd[i]-> has_infile)
			{
				int fd = open(cmd[i]->infile, O_RDONLY);
				if (fd == -1)
					exit_error("File not found");
				dup2(fd, STDIN_FILENO);
				close(fd);
			}
			//if this comman is writing to a file, output to it
			if (cmd[i]->has_outfile)
			{
				int	fd;
				if (cmd[i]->has_appendfile)
					fd = open(cmd[i]->outfile, O_WRONLY | O_CREAT | O_APPEND, 0644);
				else
					fd = open(cmd[i]->outfile, O_WRONLY | O_CREAT | O_TRUNC, 0644);
				if (fd == -1)
					exit_error("File not found");
				dup2(fd, STDOUT_FILENO);
				close(fd);
			}
			// if this command is after a pipe, input from it
			if (i > 0 && cmd[i-1]->has_pipe)
			{
				dup2(prev_pipe[0], STDIN_FILENO);
				close(prev_pipe[0]);
			}
			execve(cmd[i]->cmd_path, cmd[i]->cmd_split, path->envp);
			exit(1);
		}
		// the parent closes the pipe
		if (i > 0 && cmd[i-1]->has_pipe)
		{
			close(prev_pipe[0]);
			close(prev_pipe[1]);
		}
		// and saves the current pipe for the next command
		if (cmd[i]->has_pipe)
		{
			prev_pipe[0] = pipe_fd[0];
			prev_pipe[1] = pipe_fd[1];
		}
	}
}
