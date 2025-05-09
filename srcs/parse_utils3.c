/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_utlis3.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hassende <hassende@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/08 17:20:11 by hassende          #+#    #+#             */
/*   Updated: 2025/05/08 17:20:51 by hassende         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	set_status_skip(t_cmd **cmd, int cmd_i, int status)
{
	cmd[cmd_i]->skip_cmd = 1;
	cmd[cmd_i]->path->exit_status = status;
}
