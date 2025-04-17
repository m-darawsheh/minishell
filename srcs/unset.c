/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hassende <hassende@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/22 10:53:56 by mdarawsh          #+#    #+#             */
/*   Updated: 2025/04/17 18:35:49 by hassende         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"


static void	remove_env_var(char **envp, int index)
{
	free(envp[index]);
	while (envp[index + 1])
	{
		envp[index] = envp[index + 1];
		index++;
	}
	envp[index] = NULL;
}

void	handle_unset(t_cmd *cmd, t_cmd_path *path)
{
	int	i;
	int	j;
	int	var_len;

	if (!cmd->cmd_split[1])
		return;
	i = 1;
	while (cmd->cmd_split[i])
	{
		j = 0;
		while (path->envp[j])
		{
			var_len = 0;
			while (path->envp[j][var_len] && path->envp[j][var_len] != '=')
				var_len++;
			if (ft_strlen(cmd->cmd_split[i]) == var_len &&
				ft_strncmp(cmd->cmd_split[i], path->envp[j], var_len) == 0)
			{
				remove_env_var(path->envp, j);
				continue;
			}
			j++;
		}
		i++;
	}
}
