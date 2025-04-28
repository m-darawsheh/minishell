/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hassende <hassende@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/27 15:28:52 by hassende          #+#    #+#             */
/*   Updated: 2025/04/28 16:04:51 by hassende         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void print_file_error(char *filename)
{
	ft_putstr_fd("minishell: ", 2);
	ft_putstr_fd(filename, 2);
	ft_putstr_fd(": No such file or directory\n", 2);
}

int handle_redir_2(t_token **tokens, t_cmd **cmd,
						int *i, int cmd_i)
{
	int fd;

	fd = open(tokens[*i + 1]->value, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd == -1)
	{
		print_file_error(tokens[*i + 1]->value);
		return (0);
	}
	cmd[cmd_i]->has_outfile = 1;
	(*i)++;
	ft_strlcpy(cmd[cmd_i]->outfile, tokens[*i]->value, MAX_FILENAME);
	close(fd);
	return (1);
}

int handle_redir_3(t_token **tokens, t_cmd **cmd,
						int *i, int cmd_i)
{
	cmd[cmd_i]->has_heredoc = 1;
	(*i)++;
	if (cmd[cmd_i]->delimiter)
		free(cmd[cmd_i]->delimiter);
	cmd[cmd_i]->delimiter = ft_strdup(tokens[*i]->value);
	if (!cmd[cmd_i]->delimiter)
		return (0);
	return (1);
}
