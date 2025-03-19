/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_init.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hassende <hassende@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/03 13:57:52 by hassende          #+#    #+#             */
/*   Updated: 2025/03/13 16:13:40 by hassende         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	free_cmd_array(t_cmd **cmd, int count)
{
	int	i;

	i = 0;
	while (i < count)
	{
		if (cmd[i])
		{
			free(cmd[i]->cmd);
			free(cmd[i]->infile);
			free(cmd[i]->outfile);
			free(cmd[i]);
		}
		i++;
	}
	free(cmd);
}

static void	init_cmd_struct(t_cmd *cmd)
{
	cmd->cmd = malloc(sizeof(char) * MAX_CMD_LEN);
	cmd->infile = malloc(sizeof(char) * MAX_FILENAME);
	cmd->outfile = malloc(sizeof(char) * MAX_FILENAME);
	if (!cmd->cmd || !cmd->infile || !cmd->outfile)
	{
		free(cmd->cmd);
		free(cmd->infile);
		free(cmd->outfile);
		return ;
	}
	cmd->has_pipe = 0;
	cmd->has_infile = 0;
	cmd->has_outfile = 0;
	cmd->has_appendfile = 0;
	cmd->cmd[0] = '\0';
	cmd->infile[0] = '\0';
	cmd->outfile[0] = '\0';
}

static int	init_all_cmd_structs(t_cmd **cmd, int cmd_count)
{
	int	i;

	i = 0;
	while (i < cmd_count + 1)
	{
		init_cmd_struct(cmd[i]);
		if (!cmd[i]->cmd || !cmd[i]->infile || !cmd[i]->outfile)
			return (0); // ! should call some sort of free-exit function
		i++;
	}
	return (1);
}

static int	count_commands(char *line_read)
{
	int	i;
	int	count;

	i = 0;
	count = 0;
	while (line_read[i])
	{
		if (line_read[i] == '|')
			count++;
		i++;
	}
	return (count);
}

t_cmd	**t_cmd_malloc(char *line_read)
{
	t_cmd	**cmd;
	int		cmd_count;
	int		i;

	i = -1;
	cmd_count = count_commands(line_read);
	cmd = malloc(sizeof(t_cmd *) * (cmd_count + 2));
	if (!cmd)
		return (NULL);
	while (++i < cmd_count + 1)
	{
		cmd[i] = malloc(sizeof(t_cmd));
		if (!cmd[i])
		{
			free_cmd_array(cmd, i);
			return (NULL);
		}
	}
	cmd[i] = NULL;
	if (!init_all_cmd_structs(cmd, cmd_count))
	{
		free_cmd_array(cmd, i);
		return (NULL);
	}
	return (cmd);
}
