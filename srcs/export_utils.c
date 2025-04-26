/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdarawsh <mdarawsh@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/26 17:46:56 by mdarawsh          #+#    #+#             */
/*   Updated: 2025/04/26 17:50:07 by mdarawsh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	is_valid_identifier(char *str)
{
	int	i;

	if (!str || !*str)
		return (0);
	if (!ft_isalpha(str[0]) && str[0] != '_')
		return (0);
	i = 1;
	while (str[i] && str[i] != '=')
	{
		if (!ft_isalnum(str[i]) && str[i] != '_')
			return (0);
		i++;
	}
	return (1);
}

static int	get_name_length(char *var)
{
	int	i;

	i = 0;
	while (var[i] && var[i] != '=')
		i++;
	return (i);
}

static int	find_env_var(char **envp, char *var, int name_len)
{
	int	i;

	i = 0;
	while (envp[i])
	{
		if (ft_strncmp(envp[i], var, name_len) == 0
			&& (envp[i][name_len] == '=' || envp[i][name_len] == '\0'))
			return (i);
		i++;
	}
	return (-1);
}

static void	add_env_var(t_cmd_path *path, char *var)
{
	int		i;
	char	**new_env;

	i = 0;
	while (path->envp[i])
		i++;
	new_env = ft_calloc(i + 2, sizeof(char *));
	if (!new_env)
		return ;
	i = 0;
	while (path->envp[i])
	{
		new_env[i] = path->envp[i];
		i++;
	}
	new_env[i] = ft_strdup(var);
	if (!new_env[i])
	{
		free(new_env);
		return ;
	}
	new_env[i + 1] = NULL;
	free(path->envp);
	path->envp = new_env;
}

static void	update_env_var(t_cmd_path *path, char *var, int pos)
{
	free(path->envp[pos]);
	path->envp[pos] = ft_strdup(var);
}
