/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdarawsh <mdarawsh@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/15 18:49:26 by hassende          #+#    #+#             */
/*   Updated: 2025/04/26 17:34:08 by mdarawsh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	init_lexer(t_lexer *lexer, char *line)
{
	lexer->count = 0;
	lexer->buffer_pos = 0;
	lexer->in_quotes = 0;
	lexer->i = 0;
	lexer->line = line;
	lexer->tokens = ft_calloc((size_t)ft_strlen(line) + 1, sizeof(t_token *));
}

int	process_quotes(t_lexer *lexer)
{
	char	c;

	c = lexer->line[lexer->i];
	if ((c == '\'' || c == '\"') && (lexer->in_quotes == 0
			|| (lexer->in_quotes == 1 && c == '\'')
			|| (lexer->in_quotes == 2 && c == '\"')))
	{
		if (lexer->in_quotes == 0)
		{
			if (c == '\'')
				lexer->in_quotes = 1;
			else
				lexer->in_quotes = 2;
		}
		else
			lexer->in_quotes = 0;
		lexer->buffer[lexer->buffer_pos++] = lexer->line[lexer->i++];
		return (1);
	}
	if (lexer->in_quotes != 0)
	{
		lexer->buffer[lexer->buffer_pos++] = lexer->line[lexer->i++];
		return (1);
	}
	return (0);
}

void	add_token(t_lexer *lexer, char *value, t_token_type type)
{
	lexer->tokens[lexer->count] = malloc(sizeof(t_token));
	if (lexer->tokens[lexer->count])
	{
		lexer->tokens[lexer->count]->value = ft_strdup(value);
		lexer->tokens[lexer->count]->type = type;
		lexer->tokens[lexer->count]->quoted = 0;
		lexer->tokens[lexer->count]->from_expansion = 0;
		lexer->count++;
	}
}

void	add_word_token(t_lexer *lexer)
{
	if (lexer->buffer_pos > 0)
	{
		lexer->buffer[lexer->buffer_pos] = '\0';
		add_token(lexer, lexer->buffer, TOKEN_WORD);
		lexer->buffer_pos = 0;
	}
}

void	add_special_double_token(t_lexer *lexer)
{
	char	special_buf[3];

	special_buf[0] = lexer->line[lexer->i];
	special_buf[1] = lexer->line[lexer->i + 1];
	special_buf[2] = '\0';
	lexer->i += 2;
	if (special_buf[0] == '>')
		add_token(lexer, special_buf, TOKEN_APPEND);
	else
		add_token(lexer, special_buf, TOKEN_HEREDOC);
}
