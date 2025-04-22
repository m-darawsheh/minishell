/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_in.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hassende <hassende@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/09 13:07:20 by hassende          #+#    #+#             */
/*   Updated: 2025/04/22 19:55:34 by hassende         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_builtin(t_cmd *cmd)
{
	if (!cmd->cmd_split || cmd->cmd_split[0] == NULL)
		return (0);
	if (!ft_strcmp(cmd->cmd_split[0], "cd"))
		return (1);
	if (!ft_strcmp(cmd->cmd_split[0], "echo"))
		return (1);
	if (!ft_strcmp(cmd->cmd_split[0], "pwd"))
		return (1);
	if (!ft_strcmp(cmd->cmd_split[0], "export"))
		return (1);
	if (!ft_strcmp(cmd->cmd_split[0], "unset"))
		return (1);
	if (!ft_strcmp(cmd->cmd_split[0], "env"))
		return (1);
	if (!ft_strcmp(cmd->cmd_split[0], "exit"))
		return (1);
	return (0);
}
