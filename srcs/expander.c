/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hassende <hassende@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/20 14:28:49 by hassende          #+#    #+#             */
/*   Updated: 2025/03/23 16:37:50 by hassende         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	expand(t_token *token, int i, t_cmd_path *path)
{
	char	tmp_buffer[MAX_ENV_NAME];
	int		j;

	j = 0;
	i++;
	while (token->value[i] != '\0' && !ft_strchr(" \t$", token->value[i]))
		tmp_buffer[j++] = token->value[i++];
	tmp_buffer[j] = '\0';
	printf("%s\n", tmp_buffer);
}

static void	check_for_expansion(t_token *token, t_cmd_path *path)
{
	int	i;
	int	in_quote;

	i = 0;
	in_quote = 0;
	if (token->value[i] == '\'')
	{
		in_quote = 1;
		i++;
	}
	if (token->value[i] == '\"')
	{
		i++;
		in_quote = 2;
	}
	// if (in_quote)
	// 	remove_quotes(token);
	while (token->value[i])
	{
		if (token->value[i] == '$' && (in_quote == 0 || in_quote == 2))
			expand(token, i, path);
		i++;
	}
}

void	expander(t_token **tokens, t_cmd_path *path)
{
	int	i;

	i = -1;
	while (tokens[++i] && tokens[i]->type == TOKEN_WORD)
		check_for_expansion(tokens[i], path);
}
