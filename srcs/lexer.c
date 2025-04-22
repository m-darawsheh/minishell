/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hassende <hassende@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/15 18:46:05 by hassende          #+#    #+#             */
/*   Updated: 2025/04/22 10:05:25 by hassende         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_tokens(t_token **tokens)
{
	int	i;

	i = -1;
	if (!tokens)
		return ;
	while (tokens[++i])
	{
		free(tokens[i]->value);
		free(tokens[i]);
	}
	free(tokens);
}

static void	add_special_single_token(t_lexer *lexer)
{
	char	special_buf[2];

	special_buf[0] = lexer->line[lexer->i];
	special_buf[1] = '\0';
	lexer->i++;
	if (special_buf[0] == '|')
		add_token(lexer, special_buf, TOKEN_PIPE);
	else if (special_buf[0] == '<')
		add_token(lexer, special_buf, TOKEN_REDIR_IN);
	else
		add_token(lexer, special_buf, TOKEN_REDIR_OUT);
}

static int	process_special(t_lexer *lexer)
{
	char	c;

	c = lexer->line[lexer->i];
	if (c == '|' || c == '<' || c == '>')
	{
		add_word_token(lexer);
		if ((c == '>' || c == '<') && lexer->line[lexer->i + 1] == c)
			add_special_double_token(lexer);
		else
			add_special_single_token(lexer);
		return (1);
	}
	return (0);
}

static int	process_whitespace(t_lexer *lexer)
{
	char	c;

	c = lexer->line[lexer->i];
	if (c == ' ' || c == '\t')
	{
		add_word_token(lexer);
		lexer->i++;
		return (1);
	}
	return (0);
}

t_token	**tokenize(char *line_read, t_cmd_path *path)
{
	t_lexer	lexer;

	init_lexer(&lexer, line_read);
	if (!lexer.tokens)
		return (print_and_null("Malloc failed"));
	while (line_read[lexer.i])
	{
		if (process_quotes(&lexer))
			continue ;
		if (process_special(&lexer))
			continue ;
		if (process_whitespace(&lexer))
			continue ;
		lexer.buffer[lexer.buffer_pos++] = lexer.line[lexer.i++];
	}
	add_word_token(&lexer);
	if (lexer.in_quotes != 0)
	{
		free_tokens(lexer.tokens);
		path->exit_status = 2;
		return (print_and_null("Syntax error: unclosed quotes"));
	}
	return (lexer.tokens);
}
