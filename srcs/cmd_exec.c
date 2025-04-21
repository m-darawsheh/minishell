/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_exec.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hassende <hassende@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/03 12:48:35 by hassende          #+#    #+#             */
/*   Updated: 2025/04/21 21:53:35 by hassende         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int		process_heredocs(t_cmd **cmd);
static t_cmd	**parse_and_prepare(char *line_read, t_cmd_path *path, t_token ***tokens_head);
static void		prepare_command_splits(t_cmd **cmd, t_token **tokens);

void	exec_cmd(char *line_read, t_cmd_path *path)
{
	t_cmd	**cmd;
	t_token	**tokens;

	cmd = parse_and_prepare(line_read, path, &tokens);
	if (!cmd)
		return ;
	if (!process_heredocs(cmd))
	{
		path->exit_status = 130;
		return ;
	}
	prepare_command_splits(cmd, tokens);
	free_tokens(tokens);
	execute_command(cmd, path);
	free_cmds(cmd);
}

static	t_cmd	**parse_and_prepare(char *line_read, t_cmd_path *path, t_token ***tokens_head)
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
			free_cmds(cmd);
			return (0);
		}
	}
	return (1);
}

int	count_tokens(t_token **tokens)
{
	int	count;

	count = 0;
	while (tokens[count] && tokens[count]->type  == TOKEN_WORD)
		count++;
	return (count);
}

static void prepare_command_splits(t_cmd **cmd, t_token **tokens)
{
	int	i;
	int	cmd_idx;
	int	arg_idx;

	i = 0;
	cmd_idx = 0;
	arg_idx = 0;
	while (tokens[i])
	{
		if (tokens[i]->type == TOKEN_WORD)
		{
			if (!cmd[cmd_idx]->cmd_split)
				cmd[cmd_idx]->cmd_split = ft_calloc(count_tokens(tokens) + 1, sizeof(char*));
			cmd[cmd_idx]->cmd_split[arg_idx++] = ft_strdup(tokens[i]->value);
		}
		else if (tokens[i]->type == TOKEN_PIPE)
		{
			cmd[cmd_idx]->cmd_split[arg_idx] = NULL;
			cmd_idx++;
			arg_idx = 0;
		}
		else if (tokens[i]->type == TOKEN_REDIR_IN || tokens[i]->type == TOKEN_REDIR_OUT ||
				tokens[i]->type == TOKEN_APPEND || tokens[i]->type == TOKEN_HEREDOC)
					i++;
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
