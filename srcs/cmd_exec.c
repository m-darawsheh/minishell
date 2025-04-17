/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_exec.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hassende <hassende@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/03 12:48:35 by hassende          #+#    #+#             */
/*   Updated: 2025/04/17 18:42:55 by hassende         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int		process_heredocs(t_cmd **cmd);
static t_cmd	**parse_and_prepare(char *line_read, t_cmd_path *path);
static void		prepare_command_splits(t_cmd **cmd);

void exec_cmd(char *line_read, t_cmd_path *path)
{
	t_cmd	**cmd;

	cmd = parse_and_prepare(line_read, path);
	if (!cmd)
		return;
	if (!process_heredocs(cmd))
	{
		path->exit_status = 130;
		return ;
	}
	prepare_command_splits(cmd);
	execute_command(cmd, path);
	free_cmds(cmd);
}

static t_cmd	**parse_and_prepare(char *line_read, t_cmd_path *path)
{
	t_cmd	**cmd;
	t_token	**tokens;

	cmd = t_cmd_malloc(line_read, path);
	if (!cmd)
		return (NULL);
	tokens = tokenize(line_read);
	if (!tokens)
		return (NULL);
	expander(tokens, path);
	if (!parse_token(tokens, cmd))
	{
		free_tokens(tokens);
		return (NULL);
	}
	free_tokens(tokens);
	return (cmd);
}

static int	process_heredocs(t_cmd **cmd)
{
	int i;

	i = -1;
	while (cmd[++i])
	{
		if (cmd[i]->has_heredoc)
			handle_heredoc(cmd[i]);
		if (cmd[i]->skip_exec)
		{
			free_cmds(cmd);
			return (0);
		}
	}
	return (1);
}

static void prepare_command_splits(t_cmd **cmd)
{
	int i;

	i = -1;
	while (cmd[++i])
	{
		cmd[i]->cmd_split = ft_split(cmd[i]->cmd, ' ');
		if (!cmd[i]->cmd_split)
			exit_error("Malloc failed");
		if (cmd[i]->cmd_split[0] == NULL)
			return;
	}
}

void wait_for_children(t_cmd_path *path, t_cmd **cmd)
{
	int	status;
	int	i;
	int	last_pid;

	i = 0;
	while (cmd[i])
		i++;
	if (i == 1)
		last_pid = cmd[0]->pid;
	else
		last_pid = cmd[i - 1]->pid;
	if (waitpid(last_pid, &status, 0) > 0)
	{
		if (WIFEXITED(status))
			path->exit_status = WEXITSTATUS(status);
		else if (WIFSIGNALED(status))
			path->exit_status = 128 + WTERMSIG(status);
	}
	while (waitpid(-1, &status, 0) > 0);
}
