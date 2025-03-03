/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_exec.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hassende <hassende@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/03 12:48:35 by hassende          #+#    #+#             */
/*   Updated: 2025/03/03 14:05:41 by hassende         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	exec_cmd(char *line_read, t_cmd_path *cmd_path)
{
	t_cmd	**cmd;
	int		i;

	i = -1;
	cmd = t_cmd_malloc(line_read);
	if (!cmd)
		exit_error("Malloc failed");
	init_cmds(cmd, cmd_path, line_read);
}
