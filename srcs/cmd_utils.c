/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hassende <hassende@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/03 13:37:31 by hassende          #+#    #+#             */
/*   Updated: 2025/04/28 17:01:59 by hassende         ###   ########.fr       */
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
		if (stat(cmd->cmd_split[0], &path_stat) == 0
			&& S_ISDIR(path_stat.st_mode))
			print_dir_error(cmd->cmd_split[0]);
		cmd->cmd_path = ft_strdup(cmd->cmd_split[0]);
		return ;
	}
	i = -1;
	while (path->path[++i])
	{
		tmp = ft_strjoin(path->path[i], "/");
		cmd->cmd_path = ft_strjoin(tmp, cmd->cmd_split[0]);
		free(tmp);
		if (access(cmd->cmd_path, X_OK) == 0)
			break ;
		free(cmd->cmd_path);
	}
	if (!path->path[i])
		print_not_found(cmd, path);
}

char	**realloc_2d(char **str, int old_size, int new_size)
{
	int		i;
	char	**new;

	i = -1;
	new = malloc(new_size * sizeof(char *));
	if (!new)
	{
		free(str);
		return (NULL);
	}
	while (++i < old_size)
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
	}
	free_2d(str);
	return (new);
}
