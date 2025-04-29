/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_handle.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hassende <hassende@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/21 23:10:45 by hassende          #+#    #+#             */
/*   Updated: 2025/04/29 15:25:54 by hassende         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	print_sorted_helper(char **sorted, int count)
{
	int		i;
	int		j;
	char	*temp;

	i = -1;
	while (++i < count - 1)
	{
		j = i + 1;
		while (j < count)
		{
			if (ft_strcmp(sorted[i], sorted[j]) > 0)
			{
				temp = sorted[i];
				sorted[i] = sorted[j];
				sorted[j] = temp;
			}
			j++;
		}
	}
	i = -1;
	while (sorted[++i])
	{
		printf("declare -x %s\n", sorted[i]);
		free(sorted[i]);
	}
}

static void	print_sorted_env(t_cmd_path *path)
{
	int		i;
	int		count;
	char	**sorted;

	count = 0;
	while (path->envp[count])
		count++;
	sorted = ft_calloc(count + 1, sizeof(char *));
	if (!sorted)
		return ;
	i = 0;
	while (path->envp[i])
	{
		sorted[i] = ft_strdup(path->envp[i]);
		i++;
	}
	sorted[i] = NULL;
	print_sorted_helper(sorted, count);
	free(sorted);
}

static void	add_or_update(t_cmd_path *path, char *var, int env_pos)
{
	if (env_pos >= 0)
		update_env_var(path, var, env_pos);
	else
		add_env_var(path, var);
}

void	export_handle(t_cmd *cmd, t_cmd_path *path)
{
	int		i;
	int		env_pos;
	int		name_len;

	if (!cmd->cmd_split[1])
		return (print_sorted_env(path));
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
			continue ;
		}
		name_len = get_name_length(cmd->cmd_split[i]);
		env_pos = find_env_var(path->envp, cmd->cmd_split[i], name_len);
		if (ft_strchr(cmd->cmd_split[i], '=') || env_pos == -1)
			add_or_update(path, cmd->cmd_split[i], env_pos);
		i++;
	}
}
