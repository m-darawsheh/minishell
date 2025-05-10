/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_call.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hassende <hassende@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 20:50:04 by hassende          #+#    #+#             */
/*   Updated: 2025/05/10 13:55:17 by hassende         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	process_dollar_sign(t_token *token, int *i, t_cmd_path *path)
{
	char	*prev_value;

	prev_value = ft_strdup(token->value);
	expand(token, *i, path);
	token->from_expansion = 1;
	if (ft_strcmp(prev_value, token->value) != 0)
	{
		free(prev_value);
		(*i)--;
		return ;
	}
	free(prev_value);
}

// 0 -> no quotes || 1 -> single quotes || 2 -> double quotes
static int	update_quote_state(char c, int state)
{
	if (c == '\'' && state == 0)
		return (1);
	else if (c == '\'' && state == 1)
		return (0);
	else if (c == '\"' && state == 0)
		return (2);
	else if (c == '\"' && state == 2)
		return (0);
	return (state);
}

void	remove_quotes(t_token *token)
{
	char	*new_value;
	int		i;
	int		j;
	int		quote_state;
	int		new_state;

	new_value = ft_calloc(sizeof(char), (ft_strlen(token->value) + 1));
	if (!new_value)
		return ;
	i = -1;
	j = 0;
	quote_state = 0;
	while (token->value[++i])
	{
		new_state = update_quote_state(token->value[i], quote_state);
		if (new_state == quote_state)
			new_value[j++] = token->value[i];
		quote_state = new_state;
	}
	free(token->value);
	token->value = new_value;
}

void	expander(t_token **tokens, t_cmd_path *path)
{
	int	i;

	i = -1;
	while (tokens[++i])
		if (tokens[i]->type == TOKEN_WORD)
			check_for_expansion(tokens[i], path);
}
