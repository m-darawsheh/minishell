/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_exec_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hassende <hassende@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 17:44:35 by hassende          #+#    #+#             */
/*   Updated: 2025/04/27 16:15:52 by hassende         ###   ########.fr       */
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
