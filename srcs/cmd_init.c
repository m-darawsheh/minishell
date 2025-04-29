/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_init.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hassende <hassende@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/03 13:57:52 by hassende          #+#    #+#             */
/*   Updated: 2025/04/29 15:50:29 by hassende         ###   ########.fr       */
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

static void	init_cmd_struct(t_cmd *cmd, t_cmd_path *path)
{
	cmd->cmd = ft_calloc(MAX_CMD_LEN, sizeof(char));
	cmd->infile = ft_calloc(MAX_FILENAME, sizeof(char));
	cmd->outfile = ft_calloc(MAX_FILENAME, sizeof(char));
	cmd->path = path;
	cmd->cmd_split = NULL;
	cmd->cmd_path = NULL;
	cmd->delimiter = ft_calloc(1024, sizeof(char *));
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
	cmd->has_heredoc = 0;
	cmd->skip_exec = 0;
	cmd->heredoc_fd = -1;
	cmd->was_quoted = 0;
	cmd->heredoc_index = 0;
}

static int	init_all_cmd_structs(t_cmd **cmd, int cmd_count, t_cmd_path *path)
{
	int	i;

	i = 0;
	while (i < cmd_count + 1)
	{
		init_cmd_struct(cmd[i], path);
		if (!cmd[i]->cmd || !cmd[i]->infile || !cmd[i]->outfile)
		{
			free_cmd_array(cmd, i);
			return (0);
		}
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

t_cmd	**t_cmd_malloc(char *line_read, t_cmd_path *path)
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
	if (!init_all_cmd_structs(cmd, cmd_count, path))
	{
		free_cmd_array(cmd, i);
		return (NULL);
	}
	return (cmd);
}
