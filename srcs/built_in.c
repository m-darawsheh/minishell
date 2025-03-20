/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_in.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hassende <hassende@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/09 13:07:20 by hassende          #+#    #+#             */
/*   Updated: 2025/03/20 13:03:56 by hassende         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_builtin(t_cmd *cmd)
{
	if (!ft_strncmp(cmd->cmd_split[0], "echo", 4))
		return (1);
	if (!ft_strncmp(cmd->cmd_split[0], "pwd", 3))
		return (1);
	if (!ft_strncmp(cmd->cmd_split[0], "export", 6))
		return (1);
	if (!ft_strncmp(cmd->cmd_split[0], "unset", 5))
		return (1);
	if (!ft_strncmp(cmd->cmd_split[0], "env", 3))
		return (1);
	if (!ft_strncmp(cmd->cmd_split[0], "exit", 4))
		return (1);
	return (0);
}
