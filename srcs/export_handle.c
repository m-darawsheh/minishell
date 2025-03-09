/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_handle.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdarawsh <mdarawsh@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/09 23:43:49 by mdarawsh          #+#    #+#             */
/*   Updated: 2025/03/10 02:08:14 by mdarawsh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "minishell.h"

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
		ft_memcpy(new[i], str[i], ft_strlen(str[i]));
		i++;
	}
	free(str);
	return (new);
}

void	add_env(t_cmd_path *path , char *var)
{
	int		i;
	char	*tmp;

	i = 0;
	tmp = ft_substr(var, 7, ft_strlen(var) - 7);
	if (!tmp)
		return;
	while (path->envp[i])
		i++;
	path->envp = realloc_2d(path->envp, i , i + 1);
	if (!path->envp)
	{
		free(tmp);
		return;
	}
	printf("the var is %s\n", tmp);
	path->envp[i] = ft_strdup(tmp);
	if (!path->envp[i])
	{
		free(tmp);
		return;
	}
	path->envp[i + 1] = NULL;
	free(tmp);
}



void	export_handle(t_cmd **cmd ,t_cmd_path *path)
{
	add_env(path, cmd[0]->cmd);
}
