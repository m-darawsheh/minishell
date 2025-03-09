/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_exec.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdarawsh <mdarawsh@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/03 12:48:35 by hassende          #+#    #+#             */
/*   Updated: 2025/03/09 07:15:52 by mdarawsh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	bluitin_cheak(t_cmd **cmd)
{
	int	i;

	i = 0;
	if (cmd[1] == NULL)
	{
		if (ft_strncmp(cmd[0]->cmd, "echo", 5) == 0)
			cmd[0]->builtin = 1;
		else if (ft_strncmp(cmd[0]->cmd, "cd", 3) == 0)
			cmd[0]->builtin = 2;
		else if (ft_strncmp(cmd[0]->cmd, "pwd", 4) == 0)
			cmd[0]->builtin = 3;
		else if (ft_strncmp(cmd[0]->cmd, "export", 7) == 0)
			cmd[0]->builtin = 4;
		else if (ft_strncmp(cmd[0]->cmd, "unset", 6) == 0)
			cmd[0]->builtin = 5;
		else if (ft_strncmp(cmd[0]->cmd, "env", 4) == 0)
			cmd[0]->builtin = 6;
		else if (ft_strncmp(cmd[0]->cmd, "exit", 5) == 0)
			cmd[0]->builtin = 7;
		else
			cmd[0]->builtin = 0;
	}
}



void	exec_builtin(t_cmd **cmd, t_cmd_path *path)
{
	(void ) path;
	if (cmd[0]->builtin == 1)
		return ;
	else if (cmd[0]->builtin == 2)
		return ;
	else if (cmd[0]->builtin == 3)
		ft_pwd();
	else if (cmd[0]->builtin == 4)
		return ;
		// ft_export(cmd, path);
	else if (cmd[0]->builtin == 5)
		return ;
	else if (cmd[0]->builtin == 6)
		return ;
	else if (cmd[0]->builtin == 7)
		return ;
}


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

	bluitin_cheak(cmd);
	if (cmd[0]->builtin)
	{
		exec_builtin(cmd, path);
		return ;
	}
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
			// Redirect output to pipe if needed
			if (cmd[i]->has_pipe)
			{
				dup2(pipe_fd[1], STDOUT_FILENO);
				close(pipe_fd[0]);
				close(pipe_fd[1]);
			}
			// Redirect input from previous pipe if needed (BEFORE infile)
			if (i > 0 && cmd[i-1]->has_pipe)
			{
				dup2(prev_pipe[0], STDIN_FILENO);
				close(prev_pipe[0]);
				close(prev_pipe[1]); // Close both ends in child
			}
			// Redirect input from infile (overrides previous pipe if needed)
			if (cmd[i]->has_infile)
			{
				int fd = open(cmd[i]->infile, O_RDONLY);
				if (fd == -1)
					exit_error("File not found");
				dup2(fd, STDIN_FILENO);
				close(fd);
			}
			// Redirect output to outfile (overrides pipe if needed)
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
			execve(cmd[i]->cmd_path, cmd[i]->cmd_split, path->envp);
			exit_error("Execve failed"); // Ensure this exits on error
		}
		// Parent closes previous pipe and manages current pipe
		if (i > 0 && cmd[i-1]->has_pipe)
		{
			close(prev_pipe[0]);
			close(prev_pipe[1]);
		}
		if (cmd[i]->has_pipe)
		{
			prev_pipe[0] = pipe_fd[0];
			prev_pipe[1] = pipe_fd[1];
		}
		else
		{
			prev_pipe[0] = -1;
			prev_pipe[1] = -1;
		}
	}
	// Wait for all child processes to finish
	while (waitpid(-1, NULL, 0) > 0);
}

