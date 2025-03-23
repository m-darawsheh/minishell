/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_exec.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hassende <hassende@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/03 12:48:35 by hassende          #+#    #+#             */
/*   Updated: 2025/03/23 16:18:58 by hassende         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	print_env(t_cmd_path *path)
{
	int	i;

	i = 0;
	while (path->envp[i])
	{
		printf("%s\n", path->envp[i]);
		i++;
	}
}

void	exec_cmd(char *line_read, t_cmd_path *path)
{
	t_cmd	**cmd;
	t_token	**tokens;
	int		i;
	int		is_child;
	int		pipe_fd[2];
	int		prev_pipe[2];
	pid_t	pid;

	is_child = 0;
	i = -1;
	prev_pipe[0] = -1;
	prev_pipe[1] = -1;
	cmd = t_cmd_malloc(line_read);
	if (!cmd)
		return ;
	tokens = tokenize(line_read);
	if (!tokens)
		return ;
	expander(tokens, path);
	if (!parse_token(tokens, cmd))
	{
		free_tokens(tokens);
		return ;
	}
	free_tokens(tokens);
	while (cmd[++i])
	{
		if (cmd[i]->has_heredoc)
			handle_heredoc(cmd[i]);
		if (cmd[i]->skip_exec)
		{
			free_cmds(cmd);
			return ;
		}
	}
	i = -1;
	while (cmd[++i])
	{
		cmd[i]->cmd_split = ft_split(cmd[i]->cmd, ' ');
		if (!cmd[i]->cmd_split)
			exit_error("Malloc failed");
		if (cmd[i]->cmd_split[0] == NULL)
			return ;
		// ! each command in the pipeline should be a child proccess, run this " exit 123 | echo hi "
		// ! you'll see that it printed hi and didn't exit bash
		if (cmd[i] -> has_pipe || (i > 0 && cmd[i - 1]->has_pipe))
			is_child = 1;
		if (!is_child)
		{
			int	stdout_backup = -1;
			int	stdin_backup = -1;

			if ((cmd[i]->has_appendfile || cmd[i]->has_infile || cmd[i]->has_outfile) && (is_builtin(cmd[i])))
			{
				stdout_backup = dup(STDOUT_FILENO);
				stdin_backup = dup(STDIN_FILENO);
			}
			if (cmd[i]->has_infile && is_builtin(cmd[i]))
			{
				int fd = open(cmd[i]->infile, O_RDONLY);
				if (fd == -1)
					exit_error("File not found");
				dup2(fd, STDIN_FILENO);
				close(fd);
			}
			if (cmd[i]->has_outfile && is_builtin(cmd[i]))
			{
				int fd = open(cmd[i]->outfile, O_CREAT | O_WRONLY | O_TRUNC, 0644);
				if (fd == -1)
					exit_error("File not found");
				dup2(fd, STDOUT_FILENO);
				close(fd);
			}
			if (cmd[i]->has_appendfile && is_builtin(cmd[i]))
			{
				int fd = open(cmd[i]->outfile, O_CREAT | O_WRONLY | O_APPEND, 0644);
				if (fd == -1)
					exit_error("File not found");
				dup2(fd, STDOUT_FILENO);
				close(fd);
			}
			if (cmd[i]->has_heredoc && is_builtin(cmd[i]))
			{
				dup2(cmd[i]->heredoc_fd, STDIN_FILENO);
				close(cmd[i]->heredoc_fd);
			}
			if (!ft_strncmp(cmd[i]->cmd_split[0], "echo", 4))
			{
				do_echo(cmd[i]);
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
				continue ;
			}
			if (!ft_strncmp(cmd[i]->cmd_split[0], "exit", 4))
			{
				do_exit(cmd[i]);
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
				continue ;
			}
			if (!ft_strncmp(cmd[i]->cmd_split[0], "cd", 2))
			{
				do_cd(cmd[i], path);
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
				continue ;
			}
			// check if export not export {like this exporttttt}
			if (!ft_strncmp(cmd[i]->cmd_split[0], "export", 6))
			{
				export_handle(cmd[i], path);
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
				continue ;
			}
			if (!ft_strncmp(cmd[i]->cmd_split[0], "env", 3))
			{
				print_env(path);
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
				continue ;
			}
			//? unset is to come.
		}
		if (cmd[i]->has_pipe)
		{
			if (pipe(pipe_fd) == -1)
				exit_error("Pipe failed");
		}
		pid = fork();
		if (pid == 0)
		{
			if (cmd[i]->has_heredoc)
			{
				dup2(cmd[i]->heredoc_fd, STDIN_FILENO);
				close(cmd[i]->heredoc_fd);
			}
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
				fd = open(cmd[i]->outfile, O_WRONLY | O_CREAT | O_TRUNC, 0644);
				if (fd == -1)
					exit_error("File not found");
				dup2(fd, STDOUT_FILENO);
				close(fd);
			}
			if (cmd[i]->has_appendfile)
			{
				int fd = open(cmd[i]->outfile, O_WRONLY | O_CREAT | O_APPEND, 0644);
				if (fd == -1)
					exit_error("File not found");
				dup2(fd, STDOUT_FILENO);
				close(fd);
			}
			// ? Handle builtins in child process when in a pipeline
			if (!ft_strncmp(cmd[i]->cmd_split[0], "echo", 4))
			{
				do_echo(cmd[i]);
				exit(0);
			}
			if (!ft_strncmp(cmd[i]->cmd_split[0], "exit", 4))
			{
				do_exit(cmd[i]);
				exit (1);
			}
			if (!ft_strncmp(cmd[i]->cmd_split[0], "cd", 2))
			{
				int rtn_code = do_cd(cmd[i], path);
				if (rtn_code)
					exit (rtn_code);
				exit(0);
			}
			if (!ft_strncmp(cmd[i]->cmd_split[0], "export", 6))
			{
				export_handle(cmd[i], path);
				exit(0);
			}
			if (!ft_strncmp(cmd[i]->cmd_split[0], "env", 3))
			{
				print_env(path);
				exit(0);
			}
			// For external commands
			setup_command(cmd[i], path);
			execve(cmd[i]->cmd_path, cmd[i]->cmd_split, path->envp);
			exit_error("Execve failed");
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
	free_cmds(cmd);
}

