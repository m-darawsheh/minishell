/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hassende <hassende@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/03 13:37:31 by hassende          #+#    #+#             */
/*   Updated: 2025/04/17 19:27:18 by hassende         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	setup_command(t_cmd *cmd, t_cmd_path *path)
{
	int			i;
	char		*tmp;
	struct stat	path_stat;

	cmd->path = path;
	if (access (cmd->cmd_split[0], X_OK) == 0)
	{
		if (stat(cmd->cmd_split[0], &path_stat) == 0 &&
				S_ISDIR(path_stat.st_mode))
			print_dir_error(cmd->cmd_split[0]);
		cmd->cmd_path = ft_strdup(cmd->cmd_split[0]);
		return ;
	}
	i = 0;
	while (path->path[i])
	{
		tmp = ft_strjoin(path->path[i], "/");
		cmd->cmd_path = ft_strjoin(tmp, cmd->cmd_split[0]);
		free(tmp);
		if (access(cmd->cmd_path, X_OK) == 0)
		break ;
		free(cmd->cmd_path);
		i++;
	}
	if (!path->path[i])
		print_not_found(cmd);
}

static void	get_file(t_cmd **cmd, char *line_read, int *i, int j, int type)
{
	int	k;

	k = 0;
	while (line_read[*i] == ' ' || line_read[*i] == '\t'
			|| (type == OUTFILE && line_read[*i] == '>')
			|| (type == INFILE && line_read[*i] == '<'))
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

void	init_cmds(t_cmd **cmd, char *line)
{
	int	i;
	int	j;
	int	k;

	i = -1;
	j = 0;
	k = 0;
	while (line[++i])
	{
		if (line[i] == '|')
		{
			cmd[j]->cmd[k] = '\0';
			cmd[j++]->has_pipe = 1;
			k = 0;
		}
		else if (line[i] == '<' || line[i] == '>')
			handle_redirections(cmd, line, &i, j);
		else
			cmd[j]->cmd[k++] = line[i];
	}
	cmd[j]->cmd[k] = '\0';
}


char **realloc_2d(char **str , int old_size , int new_size)
{
	int i = 0;
	char **new;

	new = malloc(new_size * sizeof(char *));
	if (!new)
	{
		free(str);
		return (NULL);
	}
	while (i < old_size)
	{
		new[i] = malloc((ft_strlen(str[i]) + 1) * sizeof(char));
		if (!new[i])
		{
			while (i-- > 0)
				free(new[i]);
			free(new);
			free(str);
			return (NULL);
		}
		ft_strlcpy(new[i], str[i], ft_strlen(str[i]) + 1);
		i++;
	}
	free(str);
	return (new);
}
