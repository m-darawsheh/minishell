/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_handle.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hassende <hassende@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/21 23:10:45 by hassende          #+#    #+#             */
/*   Updated: 2025/04/22 20:28:21 by hassende         ###   ########.fr       */
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
		if (ft_strncmp(envp[i], var, name_len) == 0 &&
			(envp[i][name_len] == '=' || envp[i][name_len] == '\0'))
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
		return;
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
		return;
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

static void	print_sorted_env(t_cmd_path *path)
{
	int		i;
	int		j;
	int		count;
	char	**sorted;
	char	*temp;

	count = 0;
	while (path->envp[count])
		count++;
	sorted = ft_calloc(count + 1, sizeof(char *));
	if (!sorted)
		return;
	i = 0;
	while (path->envp[i])
	{
		sorted[i] = ft_strdup(path->envp[i]);
		i++;
	}
	sorted[i] = NULL;
	i = 0;
	while (i < count - 1)
	{
		j = i + 1;
		while (j < count )
		{
			if (ft_strcmp(sorted[i], sorted[j]) > 0)
			{
				temp = sorted[i];
				sorted[i] = sorted[j];
				sorted[j] = temp;
			}
			j++;
		}
		i++;
	}
	i = 0;
	while (sorted[i])
	{
		printf("declare -x %s\n", sorted[i]);
		free(sorted[i]);
		i++;
	}
	free(sorted);
}

void	export_handle(t_cmd *cmd, t_cmd_path *path)
{
	int		i;
	int		env_pos;
	int		name_len;

	if (!cmd->cmd_split[1])
	{
		print_sorted_env(path);
		return;
	}
	i = 1;
	while (cmd->cmd_split[i])
	{
		if (!is_valid_identifier(cmd->cmd_split[i]))
		{
			ft_putstr_fd("export: `", 2);
			ft_putstr_fd(cmd->cmd_split[i], 2);
			ft_putstr_fd("': not a valid identifier\n", 2);
			path->exit_status = 1;
			i++;
			continue;
		}
		name_len = get_name_length(cmd->cmd_split[i]);
		env_pos = find_env_var(path->envp, cmd->cmd_split[i], name_len);
		if (ft_strchr(cmd->cmd_split[i], '=') || env_pos == -1)
		{
			if (env_pos >= 0)
				update_env_var(path, cmd->cmd_split[i], env_pos);
			else
				add_env_var(path, cmd->cmd_split[i]);
		}
		i++;
	}
}
