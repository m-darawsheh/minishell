/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_exec.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hassende <hassende@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/03 12:48:35 by hassende          #+#    #+#             */
/*   Updated: 2025/03/15 15:27:34 by hassende         ###   ########.fr       */
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

// void	bluitin_cheak(t_cmd **cmd)
// {

// 	if (cmd[1] == NULL)
// 	{
// 		if (ft_strncmp(cmd[0]->cmd, "echo", 4) == 0)
// 			cmd[0]->builtin = 1;
// 		else if (ft_strncmp(cmd[0]->cmd, "cd", 2) == 0)
// 			cmd[0]->builtin = 2;
// 		else if (ft_strncmp(cmd[0]->cmd, "pwd", 3) == 0)
// 			cmd[0]->builtin = 3;
// 		else if (ft_strncmp(cmd[0]->cmd, "export", 6) == 0)
// 			cmd[0]->builtin = 4;
// 		else if (ft_strncmp(cmd[0]->cmd, "unset", 5) == 0)
// 			cmd[0]->builtin = 5;
// 		else if (ft_strncmp(cmd[0]->cmd, "env", 3) == 0)
// 			cmd[0]->builtin = 6;
// 		else if (ft_strncmp(cmd[0]->cmd, "exit", 4) == 0)
// 			cmd[0]->builtin = 7;
// 		else
// 			cmd[0]->builtin = 0;
// 	}
// }



// void	exec_builtin(t_cmd **cmd, t_cmd_path *path)
// {
// 	(void ) path;
// 	if (cmd[0]->builtin == 1)
// 		return ;
// 	else if (cmd[0]->builtin == 2)
// 		return ;
// 	else if (cmd[0]->builtin == 3)
// 		pwd_handle();
// 	else if (cmd[0]->builtin == 4)
// 		export_handle(cmd, path);
// 	else if (cmd[0]->builtin == 5)
// 		return ;
// 	else if (cmd[0]->builtin == 6)
// 		print_env(path);
// 	else if (cmd[0]->builtin == 7)
// 		return ;
// }

//TODO exit - builtin command
//TODO echo - with -n flag
//TODO heredoc - <<

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
		exit_error("Malloc failed");
	tokens = tokenize(line_read);
	if (!tokens)
		exit_error("Malloc failed");

	parse_tokens(tokens, cmd);
	free_tokens(tokens);
	// init_cmds(cmd, line_read);


	// bluitin_cheak(cmd);


	// if (cmd[0]->builtin)
	// {
	// 	exec_builtin(cmd, path);
	// 	return ;
	// }

	while (cmd[++i])
	{
		cmd[i]->cmd_split = ft_split (cmd[i]->cmd, ' ');
		if (!cmd[i]->cmd_split)
			exit_error("Malloc failed");
		// ! each command in the pipeline should be a child proccess, run this " exit 123 | echo hi "
		// ! you'll see that it printed hi and didn't exit minishell
		if (cmd[i] -> has_pipe || (i > 0 && cmd[i - 1]->has_pipe))
			is_child = 1;

		if (!is_child)
		{
			if (!ft_strncmp(cmd[i]->cmd_split[0], "echo", 4))
			{
				do_echo(cmd[i]);
				continue ;
			}
			if (!ft_strncmp(cmd[i]->cmd_split[0], "exit", 4))
				if(!do_exit(cmd[i]))
					continue ;
			if (!ft_strncmp(cmd[i]->cmd_split[0], "cd", 2))
			{
				do_cd(cmd[i], path);
				continue ;
			}
			// check if export not export {like this exporttttt}
			if (!ft_strncmp(cmd[i]->cmd_split[0], "export", 6))
			{
				export_handle(cmd[i], path);
				continue ;
			}
			if (!ft_strncmp(cmd[i]->cmd_split[0], "env", 3))
			{
				print_env(path);
				continue ;
			}
		}
		if (cmd[i]->has_pipe)
		{
			if (pipe(pipe_fd) == -1)
			exit_error("Pipe failed");
		}

		pid = fork();
		if (pid == 0)
		{
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
}

