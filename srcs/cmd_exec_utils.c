/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_exec_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hassende <hassende@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 17:44:35 by hassende          #+#    #+#             */
/*   Updated: 2025/04/28 16:57:13 by hassende         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_not_word(t_token_type type)
{
	if (type == TOKEN_REDIR_IN || type == TOKEN_REDIR_OUT
		|| type == TOKEN_APPEND || type == TOKEN_HEREDOC)
		return (1);
	return (0);
}

int	count_tokens(t_token **tokens)
{
	int	count;

	count = 0;
	while (tokens[count] && tokens[count]->type == TOKEN_WORD)
		count++;
	return (count);
}

int	count_command_tokens(t_token **tokens, int start_idx)
{
	int	j;
	int	token_count;

	token_count = 0;
	j = start_idx;
	while (tokens[j] && tokens[j]->type != TOKEN_PIPE)
	{
		if (tokens[j]->type == TOKEN_WORD)
			token_count++;
		j++;
	}
	return (token_count);
}

void	setter_norm(t_cmd **cmd, int *cmd_idx, int *arg_idx)
{
	cmd[*cmd_idx]->cmd_split[*arg_idx] = NULL;
	(*cmd_idx)++;
	*arg_idx = 0;
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
