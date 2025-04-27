/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_exec.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hassende <hassende@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/03 12:48:35 by hassende          #+#    #+#             */
/*   Updated: 2025/04/27 16:15:44 by hassende         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int		process_heredocs(t_cmd **cmd);
static t_cmd	**parse_and_prepare(char *line_read, t_cmd_path *path,
					t_token ***tokens_head);
static void		prepare_command_splits(t_cmd **cmd, t_token **tokens);

void	exec_cmd(char *line_read, t_cmd_path *path)
{
	t_cmd	**cmd;
	t_token	**tokens;

	cmd = parse_and_prepare(line_read, path, &tokens);
	if (!cmd)
	{
		free_cmds(cmd, 0);
		return ;
	}
	if (!process_heredocs(cmd))
	{
		path->exit_status = 130;
		return ;
	}
	prepare_command_splits(cmd, tokens);
	free_tokens(tokens);
	execute_command(cmd, path);
	free_cmds(cmd, 0);
}

static	t_cmd	**parse_and_prepare(char *line_read, t_cmd_path *path,
		t_token ***tokens_head)
{
	t_cmd	**cmd;
	t_token	**tokens;

	cmd = t_cmd_malloc(line_read, path);
	if (!cmd)
		return (NULL);
	tokens = tokenize(line_read, path);
	if (!tokens)
	{
		free_cmds(cmd, 0);
		return (NULL);
	}
	expander(tokens, path);
	if (!parse_token(tokens, cmd))
	{
		free_tokens(tokens);
		return (NULL);
	}
	*tokens_head = tokens;
	return (cmd);
}

static int	process_heredocs(t_cmd **cmd)
{
	int	i;

	i = -1;
	while (cmd[++i])
	{
		if (cmd[i]->has_heredoc)
			handle_heredoc(cmd[i]);
		if (cmd[i]->skip_exec)
		{
			free_cmds(cmd, 0);
			return (0);
		}
	}
	return (1);
}

static void	prepare_command_splits(t_cmd **cmd, t_token **tokens)
{
	int	i;
	int	cmd_idx;
	int	arg_idx;

	i = -1;
	cmd_idx = 0;
	arg_idx = 0;
	while (tokens[++i])
	{
		if (tokens[i]->type == TOKEN_WORD)
		{
			if (!cmd[cmd_idx]->cmd_split)
				cmd[cmd_idx]->cmd_split = ft_calloc(
						count_command_tokens(tokens, i) + 1, sizeof (char *));
			cmd[cmd_idx]->cmd_split[arg_idx++] = ft_strdup(tokens[i]->value);
			if (tokens[i]->quoted == 1)
				cmd[cmd_idx]->was_quoted = 1;
		}
		else if (tokens[i]->type == TOKEN_PIPE)
			setter_norm(cmd, &cmd_idx, &arg_idx);
		else if (is_not_word(tokens[i]->type))
			i++;
	}
	cmd[cmd_idx]->cmd_split[arg_idx] = NULL;
}

void	wait_for_children(t_cmd_path *path, t_cmd **cmd)
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
	while (waitpid(-1, &status, 0) > 0)
		;
}
