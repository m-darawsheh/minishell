/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hassende <hassende@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/03 13:37:31 by hassende          #+#    #+#             */
/*   Updated: 2025/03/03 13:43:55 by hassende         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	get_file(t_cmd **cmd, char *line_read, int *i, int j, int type)
{
	int	k;

	k = 0;
	while (line_read[*i] == ' ')
		(*i)++;
	while (line_read[*i] != ' ' && line_read[*i] != '|' && line_read[*i] != '\0')
	{
		if (type == INFILE)
			cmd[j]->infile[k] = line_read[*i];
		else
			cmd[j]->outfile[k] = line_read[*i];
		(*i)++;
		k++;
	}
	if (type == INFILE)
		cmd[j]->infile[k] = '\0';
	else
		cmd[j]->outfile[k] = '\0';
}

static void	handle_redirections(t_cmd **cmd, char *line, int *i, int j)
{
	if (line[*i] == '<')
	{
		cmd[j]->has_infile = 1;
		(*i)++;
		get_file(cmd, line, i, j, INFILE);
	}
	else if (line[*i] == '>')
	{
		cmd[j]->has_outfile = 1;
		if (line[++(*i)] == '>')
			cmd[j]->has_appendfile = 1;
		get_file(cmd, line, i, j, OUTFILE);
	}
}

void	init_cmds(t_cmd **cmd, t_cmd_path *cmd_path, char *line)
{
	int	i;
	int	j;
	int	k;

	i = 0;
	j = 0;
	k = 0;
	while (line[i])
	{
		if (line[i] == '|')
		{
			cmd[++j]->has_pipe = 1;
			k = 0;
		}
		else if (line[i] == '<' || line[i] == '>')
			handle_redirections(cmd, line, &i, j);
		else
			cmd[j]->cmd[k++] = line[i];
		i++;
	}
}
