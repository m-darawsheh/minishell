/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path_init.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hassende <hassende@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/25 18:23:27 by hassende          #+#    #+#             */
/*   Updated: 2025/04/16 18:45:17 by hassende         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	print_env(t_cmd_path *path)
{
	int	i;

	i = 0;
	while (path->envp[i])
	{
		printf("%s\n", path->envp[i]);
		i++;
	}
}

char	**find_path(char **envp)
{
	int		i;
	char	**path;

	i = 0;
	while (envp[i])
	{
		if (ft_strncmp(envp[i], "PATH=", 5) == 0)
		{
			path = ft_split(envp[i] + 5, ':');
			return (path);
		}
		i++;
	}
	return (NULL);
}

char	**get_envp (char **envp)
{
	int		i;
	char	**tmp;
	i = 0;
	while (envp[i])
		i++;
	tmp = malloc(sizeof(char *) * (i + 1));
	if (!tmp)
	{
		free(envp);
		perror("Malloc failed");
		exit(1);
	}
	i = 0;
	while (envp[i])
	{
		tmp[i] = ft_strdup(envp[i]);
		i++;
	}
	tmp[i] = NULL;
	return (tmp);
}

void	struc_init(t_cmd_path *path, char **envp)
{
	// path->envp = envp;
	path->exit_status = 0;
	path-> envp = get_envp(envp);
	path->path = find_path(path->envp);
	if (!path->path)
	{
		free_2d(path->envp);
		exit_error("PATH not found in envp");
	}
}
