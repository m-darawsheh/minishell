/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdarawsh <mdarawsh@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/22 10:53:56 by mdarawsh          #+#    #+#             */
/*   Updated: 2025/03/22 11:15:14 by mdarawsh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"


void	handle_unset(t_cmd *cmd, t_cmd_path *path)
{
	int i;

	i = 1;
	if (cmd->cmd_split[1] == NULL)
	{
		printf("minishell: unset: not enough arguments\n");
		return ;
	}
	else
	{
		while (cmd->cmd_split[i])
		{
			if (ft_strncmp(cmd->cmd_split[i], path->envp[i], ft_strlen(cmd->cmd_split[i])) == 0)
			{
				free(path->envp[i]);
				path->envp[i] = NULL;
			}
			i++;
		}
	}
	return ;
}
