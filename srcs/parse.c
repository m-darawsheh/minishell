/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hassende <hassende@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/16 13:06:27 by hassende          #+#    #+#             */
/*   Updated: 2025/03/16 14:09:50 by hassende         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	parse_token(t_token **tokens, t_cmd **cmd)
{
	int	i;
	int	cmd_i;

	i = -1;
	cmd_i = 0;
	cmd[0]->cmd[0] = '\0';
	while (tokens[++i])
	{
		if (tokens[i]->type == TOKEN_WORD)
		{
			if (cmd[cmd_i]->cmd[0] != '\0')
				ft_strlcat(cmd[cmd_i]->cmd, " ", MAX_CMD_LEN);
			ft_strlcat(cmd[cmd_i]->cmd, tokens[i]->value, MAX_CMD_LEN);
			i++;
		}
		if (tokens[i]->type == TOKEN_PIPE)
		{
			cmd[cmd_i]->has_pipe = 1;
			cmd_i++;
			cmd[cmd_i]->cmd[0] = '\0';
			i++;
		}
		if (tokens[i]->type == TOKEN_REDIR_IN)
		{
			cmd[cmd_i]->has_infile = 1;
			i++;
			if (tokens[i] && tokens[i]->type == TOKEN_WORD)
			{
				ft_strlcpy(cmd[cmd_i]->infile, tokens[i]->value, MAX_FILENAME);
				i++;
			}
			else
			{
				ft_putstr_fd("minishell: syntax error near unexpected token 'newline'\n", 2);
				return (0);
			}
		}
		if (tokens[i]->type == TOKEN_REDIR_OUT)
		{
			cmd[cmd_i]->has_outfile = 1;
			i++;
			if (tokens[i] && tokens[i]->type == TOKEN_WORD)
			{
				ft_strlcpy(cmd[cmd_i]->infile, tokens[i]->value, MAX_FILENAME);
				i++;
			}
			else
			{
				ft_putstr_fd("minishell: syntax error near unexpected token `newline'\n", 2);
				return (0);
			}
		}
		if (tokens[i]->type == TOKEN_APPEND)
		{
			cmd[cmd_i]->has_appendfile = 1;
			i++;
			if (tokens[i] && tokens[i]->type == TOKEN_WORD)
			{
				ft_strlcpy(cmd[cmd_i]->infile, tokens[i]->value, MAX_FILENAME);
				i++;
			}
			else
			{
				ft_putstr_fd("minishell: syntax error near unexpected token `newline'\n", 2);
				return (0);
			}
		}
		if (tokens[i]->type == TOKEN_HEREDOC)
		{
			cmd[cmd_i]->has_heredoc = 1;
			i++;
			if (tokens[i] && tokens[i]->type == TOKEN_WORD)
			{
				ft_strlcpy(cmd[cmd_i]->delimiter, tokens[i]->value, MAX_FILENAME);
				i++;
			}
			else
			{
				ft_putstr_fd("minishell: syntax error near unexpected token `newline'\n", 2);
				return (0);
			}
		}
	}
	return (1);
}
