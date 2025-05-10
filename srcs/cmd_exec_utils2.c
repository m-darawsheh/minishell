/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_exec_utils2.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hassende <hassende@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/10 13:52:28 by hassende          #+#    #+#             */
/*   Updated: 2025/05/10 13:56:31 by hassende         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	is_empty_expanded_var(t_token *token)
{
	return (token->from_expansion && token->value[0] == '\0');
}

static void	init_cmd_split(t_cmd *cmd, t_token **tokens, int i)
{
	if (!cmd->cmd_split)
		cmd->cmd_split = ft_calloc(
				count_command_tokens(tokens, i) + 1, sizeof(char *));
}

static void	add_token_to_cmd(t_cmd *cmd, t_token *token, int *arg_idx)
{
	cmd->cmd_split[(*arg_idx)++] = ft_strdup(token->value);
	if (token->quoted == 1)
		cmd->was_quoted = 1;
}

static void	process_word_token(t_cmd *cmd, int *arg_idx,
		t_token **tokens, int i)
{
	if (!(*arg_idx == 0 && is_empty_expanded_var(tokens[i])))
	{
		init_cmd_split(cmd, tokens, i);
		add_token_to_cmd(cmd, tokens[i], arg_idx);
	}
}

void	prepare_command_splits(t_cmd **cmd, t_token **tokens)
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
			process_word_token(cmd[cmd_idx], &arg_idx, tokens, i);
		else if (tokens[i]->type == TOKEN_PIPE)
			setter_norm(cmd, &cmd_idx, &arg_idx);
		else if (is_not_word(tokens[i]->type))
			i++;
	}
	if (!cmd[cmd_idx]->cmd_split)
		cmd[cmd_idx]->cmd_split = ft_calloc(1, sizeof(char *));
}
