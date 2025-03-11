/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_handle.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdarawsh <mdarawsh@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/09 23:43:49 by mdarawsh          #+#    #+#             */
/*   Updated: 2025/03/10 03:44:58 by mdarawsh         ###   ########.fr       */
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
	path->envp[i] = ft_strdup(tmp);
	if (!path->envp[i])
	{
		free(tmp);
		return;
	}
	path->envp[i + 1] = NULL;
	free(tmp);
}

int check_env(t_cmd_path *path , char *var, int *i)
{
	int j;

	j = 0;
	while (var[j] != '=')
		j++;
	// printf("check var is %s\n", var + 7);
	while (path->envp[*i])
	{
		if (ft_strncmp(path->envp[*i], var + 7 , j - 7) == 0)
			return (*i);
		(*i)++;
	}
	return (0);
}

// export VAR=VALUE
void	edit_env(t_cmd_path *path , char *var, int *i)
{
	int j;
	int k;

	j = 0;
	k = 0;
	while (var[j] != '=')
		j++;
	while (path->envp[*i][k] != '=')
		k++;
	while (var[j])
		path->envp[*i][k++] = var[j++];
	path->envp[*i][k] = '\0';
}


void	export_handle(t_cmd **cmd ,t_cmd_path *path)
{
	int i;

	i = 0;
	if (check_env(path, cmd[0]->cmd, &i))
		edit_env(path, cmd[0]->cmd, &i);
	else
		add_env(path, cmd[0]->cmd);
}
