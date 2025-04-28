/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hassende <hassende@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/16 13:06:27 by hassende          #+#    #+#             */
/*   Updated: 2025/04/28 16:51:24 by hassende         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	handle_pipe(t_token **tokens, t_cmd **cmd, int *i, int *cmd_i)
{
	if (tokens[*i + 1] == NULL || tokens[*i + 1]->type != TOKEN_WORD)
	{
		ft_putstr_fd("minishell: syntax error near token `newline'\n", 2);
		cmd[*cmd_i]->path->exit_status = 2;
		return (0);
	}
	cmd[*cmd_i]->has_pipe = 1;
	cmd[++*cmd_i]->cmd[0] = '\0';
	return (1);
}

static int	handle_expansion_pipe(t_token **tokens, t_cmd **cmd,
									int i, int cmd_i)
{
	tokens[i]->type = TOKEN_WORD;
	handle_word(tokens, cmd, i, cmd_i);
	return (1);
}

static int	process_token(t_token **tokens, t_cmd **cmd, int *i, int *cmd_i)
{
	if (tokens[*i]->type == TOKEN_WORD)
		handle_word(tokens, cmd, *i, *cmd_i);
	else if (tokens[*i]->from_expansion && tokens[*i]->value[0] == '|')
		return (handle_expansion_pipe(tokens, cmd, *i, *cmd_i));
	else if (tokens[*i]->type == TOKEN_PIPE)
	{
		if (!handle_pipe(tokens, cmd, i, cmd_i))
			return (0);
	}
	else if ((tokens[*i]->type == TOKEN_REDIR_IN
			|| tokens[*i]->type == TOKEN_REDIR_OUT)
		&& !handle_redir(tokens, cmd, i, *cmd_i))
		return (0);
	else if ((tokens[*i]->type == TOKEN_APPEND
			|| tokens[*i]->type == TOKEN_HEREDOC)
		&& !handle_advanced_redir(tokens, cmd, i, *cmd_i))
		return (0);
	return (1);
}

int	parse_token(t_token **tokens, t_cmd **cmd)
{
	int	i;
	int	cmd_i;

	i = -1;
	cmd_i = 0;
	cmd[0]->cmd[0] = '\0';
	while (tokens[++i])
	{
		if (!process_token(tokens, cmd, &i, &cmd_i))
			return (0);
	}
	return (1);
}
