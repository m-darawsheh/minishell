/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hassende <hassende@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/16 13:06:27 by hassende          #+#    #+#             */
/*   Updated: 2025/03/26 13:41:23 by hassende         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	handle_word(t_token **tokens, t_cmd **cmd, int i, int cmd_i)
{
	if (cmd[cmd_i]->cmd[0] != '\0')
		ft_strlcat(cmd[cmd_i]->cmd, " ", MAX_CMD_LEN);
	ft_strlcat(cmd[cmd_i]->cmd, tokens[i]->value, MAX_CMD_LEN);
}

static int	handle_redir(t_token **tokens, t_cmd **cmd,
						int *i, int cmd_i)
{
	if (!tokens[*i + 1] || tokens[*i + 1]->type != TOKEN_WORD)
	{
		ft_putstr_fd("minishell: syntax error near token `newline'\n", 2);
		cmd[cmd_i]->path->exit_status = 2;
		return (0);
	}
	if (tokens[*i]->type == TOKEN_REDIR_IN)
	{
		int fd = open(tokens[*i + 1]->value, O_RDONLY);
		if (fd == -1)
		{
			ft_putstr_fd("minishell: ", 2);
			ft_putstr_fd(tokens[*i + 1]->value, 2);
			ft_putstr_fd(": No such file or directory\n", 2);
			return (0);
		}
		close(fd);
		cmd[cmd_i]->has_infile = 1;
		(*i)++;
		ft_strlcpy(cmd[cmd_i]->infile, tokens[*i]->value, MAX_FILENAME);
	}
	else if (tokens[*i]->type == TOKEN_REDIR_OUT)
	{
		int fd = open(tokens[*i + 1]->value, O_WRONLY | O_CREAT | O_TRUNC, 0644);
		if (fd == -1)
			return (0);
		cmd[cmd_i]->has_outfile = 1;
		(*i)++;
		ft_strlcpy(cmd[cmd_i]->outfile, tokens[*i]->value, MAX_FILENAME);
		close(fd);
	}
	return (1);
}

static int	handle_advanced_redir(t_token **tokens, t_cmd **cmd,
								int *i, int cmd_i)
{
	if (!tokens[*i + 1] || tokens[*i + 1]->type != TOKEN_WORD)
	{
		ft_putstr_fd("minishell: syntax error near token \'>> / <<\'\n", 2);
		cmd[cmd_i]->path->exit_status = 2;
		return (0);
	}
	if (tokens[*i]->type == TOKEN_APPEND)
	{
		int fd = open(tokens[*i + 1]->value, O_WRONLY | O_CREAT | O_APPEND, 0644);
		if (fd == -1)
			return (0);
		cmd[cmd_i]->has_appendfile = 1;
		(*i)++;
		ft_strlcpy(cmd[cmd_i]->outfile, tokens[*i]->value, MAX_FILENAME);
		close(fd);
	}
	else if (tokens[*i]->type == TOKEN_HEREDOC)
	{
		cmd[cmd_i]->has_heredoc = 1;
		(*i)++;
		cmd[cmd_i]->delimiter = ft_strdup(tokens[*i]->value);
		if (!cmd[cmd_i]->delimiter)
			return (0);
	}
	return (1);
}

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
			handle_word(tokens, cmd, i, cmd_i);
		else if (tokens[i]->type == TOKEN_PIPE)
		{
			if(!handle_pipe(tokens, cmd, &i, &cmd_i))
				return (0);
		}
		else if ((tokens[i]->type == TOKEN_REDIR_IN
			|| tokens[i]->type == TOKEN_REDIR_OUT)
			&& !handle_redir(tokens, cmd, &i, cmd_i))
			return (0);
		else if ((tokens[i]->type == TOKEN_APPEND
			|| tokens[i]->type == TOKEN_HEREDOC)
			&& !handle_advanced_redir(tokens, cmd, &i, cmd_i))
			return (0);
	}
	return (1);
}
