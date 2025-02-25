/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hassende <hassende@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/25 16:19:37 by hassende          #+#    #+#             */
/*   Updated: 2025/02/25 17:27:21 by hassende         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	exit_error(char *str)
{
	ft_putendl_fd(str, 2);
	exit(1);
}

void	free_all(t_cmd *cmd)
{
	free_2d(cmd->cmd_split);
	free_2d(cmd->path);
	rl_clear_history();
	if (cmd->cmd)
		free(cmd->cmd);
}
