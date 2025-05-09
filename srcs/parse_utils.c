/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hassende <hassende@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/27 15:28:52 by hassende          #+#    #+#             */
/*   Updated: 2025/05/08 18:12:28 by hassende         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	print_file_error(char *filename)
{
	char	*temp;
	char	*temp2;

	temp = ft_strjoin(filename, ": No such file or directory\n");
	if (!temp)
		return ;
	temp2 = ft_strjoin("minishell: ", temp);
	free(temp);
	if (!temp2)
		return ;
	write(2, temp2, ft_strlen(temp2));
	free(temp2);
}

int	handle_redir_2(t_token **tokens, t_cmd **cmd,
						int *i, int cmd_i)
{
	int	fd;

	fd = open(tokens[*i + 1]->value, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd == -1)
	{
		print_file_error(tokens[*i + 1]->value);
		set_status_skip(cmd, cmd_i, 1);
		return (0);
	}
	cmd[cmd_i]->has_outfile = 1;
	(*i)++;
	ft_strlcpy(cmd[cmd_i]->outfile, tokens[*i]->value, MAX_FILENAME);
	close(fd);
	return (1);
}

int	handle_redir_3(t_token **tokens, t_cmd **cmd,
						int *i, int cmd_i)
{
	cmd[cmd_i]->has_heredoc = 1;
	(*i)++;
	cmd[cmd_i]->delimiter[cmd[cmd_i]->heredoc_index] = ft_strdup
		(tokens[*i]->value);
	if (!cmd[cmd_i]->delimiter[cmd[cmd_i]->heredoc_index])
		return (0);
	cmd[cmd_i]->heredoc_index++;
	return (1);
}
