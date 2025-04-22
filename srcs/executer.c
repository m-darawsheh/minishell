/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executer.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hassende <hassende@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/16 18:42:30 by hassende          #+#    #+#             */
/*   Updated: 2025/04/22 18:27:50 by hassende         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	execute_command(t_cmd **cmd, t_cmd_path *path)
{
	int		i;
	int		is_child;
	int		pipe_fd[2];
	int		prev_pipe[2];
	int		stdin_backup;
	int		stdout_backup;
	int		got_forked;

	i = -1;
	prev_pipe[0] = -1;
	prev_pipe[1] = -1;
	got_forked = 0;
	while (cmd[++i])
	{
		is_child = cmd[i]->has_pipe || (i > 0 && cmd[i - 1]->has_pipe);
		if (!is_child)
		{
			stdin_backup = -1;
			stdout_backup = -1;
			if ((cmd[i]->has_appendfile || cmd[i]->has_infile
				|| cmd[i]->has_outfile) && is_builtin(cmd[i]))
			{
				stdout_backup = dup(STDOUT_FILENO);
				stdin_backup = dup(STDIN_FILENO);
				setup_io_redirections(cmd[i]);
			}
			if (cmd[i]->cmd_split && cmd[i]->cmd_split[0] && ft_strchr(cmd[i]->cmd_split[0], ' ') && cmd[i]->was_quoted != 1)
			{
				char **new_args = ft_split(cmd[i]->cmd_split[0], ' ');
				if (new_args)
				{
					if (cmd[i]->cmd_split[1])
						append_args(&new_args, cmd[i]);
					free_2d(cmd[i]->cmd_split);
					cmd[i]->cmd_split = new_args;
				}
			}
			if (is_builtin(cmd[i]))
			{
				execute_builtin(cmd[i], path, stdin_backup, stdout_backup);
				continue ;
			}
		}
		got_forked = 1;
		if (cmd[i]->has_pipe)
			if (pipe(pipe_fd) == -1)
				exit_error("Pipe failed");
		cmd[i]->pid = fork();
		if (cmd[i]->pid == 0)
		{
			setup_io_redirections_child(cmd[i], pipe_fd, prev_pipe, i);
			setup_exec_signals();
			execute_builtin_child(cmd[i], path);
		}
		handle_pipes(pipe_fd, prev_pipe, cmd, i);
	}
	setup_interactive_signals();
	if (got_forked)
		wait_for_children(path, cmd);
}

void	execute_builtin(t_cmd *cmd, t_cmd_path *path,
							int stdin_backup, int stdout_backup)
{
	if (!ft_strncmp(cmd->cmd_split[0], "echo", 4))
	{
		path->exit_status = 0;
		do_echo(cmd);
	}
	else if (!ft_strncmp(cmd->cmd_split[0], "exit", 4))
		path->exit_status = do_exit(cmd);
	else if (!ft_strncmp(cmd->cmd_split[0], "cd", 2))
	{
		path->exit_status = do_cd(cmd, path);
	}
	else if (!ft_strncmp(cmd->cmd_split[0], "export", 6))
	{
		path->exit_status = 0;
		export_handle(cmd, path);
	}
	else if (!ft_strncmp(cmd->cmd_split[0], "env", 3))
	{
		path->exit_status = 0;
		print_env(path);
	}
	else if (!ft_strncmp(cmd->cmd_split[0], "pwd", 3))
	{
		path->exit_status = 0;
		pwd_handle(path);
	}
	else if (!ft_strncmp(cmd->cmd_split[0], "unset", 5))
	{
		path->exit_status = 0;
		handle_unset(cmd, path);
	}
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

static void	do_builtin_children(t_cmd *cmd, t_cmd_path *path)
{
	int	rtn_code;

	if (!ft_strncmp(cmd->cmd_split[0], "echo", 4))
	{
		do_echo(cmd);
		exit(0);
	}
	else if (!ft_strncmp(cmd->cmd_split[0], "exit", 4))
	{
		do_exit(cmd);
		exit(1);
	}
	else if (!ft_strncmp(cmd->cmd_split[0], "cd", 2))
	{
		rtn_code = do_cd(cmd, path);
		exit(rtn_code);
	}
	else if (!ft_strncmp(cmd->cmd_split[0], "export", 6))
	{
		export_handle(cmd, path);
		exit(0);
	}
	else if (!ft_strncmp(cmd->cmd_split[0], "env", 3))
	{
		print_env(path);
		exit(0);
	}
}

void append_args(char ***new_args, t_cmd *cmd)
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
		return;
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

int	expanded_as_command(t_cmd *cmd)
{
	char	**new_args;

	if (!ft_strchr(cmd->cmd_split[0], ' '))
		return (0);
	new_args = ft_split(cmd->cmd_split[0], ' ');
	setup_command(cmd, cmd->path);
	if (!new_args)
		return (0);
	if (cmd->cmd_split[1])
		append_args(&new_args, cmd);
	if (!new_args)
		return (0);
	free_2d(cmd->cmd_split);
	cmd->cmd_split = new_args;
	return (1);
}

// ! WOW - very demure
void	wow()
{
	return ;
}

void	execute_builtin_child(t_cmd *cmd, t_cmd_path *path)
{
	// if (cmd->cmd_split || cmd->cmd_split[0])
		do_builtin_children(cmd, path);
	if (expanded_as_command(cmd))
		wow();
	else
		setup_command(cmd, path);
	execve(cmd->cmd_path, cmd->cmd_split, path->envp);
	exit(127);
}
