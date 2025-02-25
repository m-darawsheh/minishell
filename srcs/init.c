/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hassende <hassende@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/25 18:23:27 by hassende          #+#    #+#             */
/*   Updated: 2025/02/25 18:23:39 by hassende         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	struc_init(t_cmd *cmd)
{
	cmd->cmd = NULL;
	cmd->cmd_split = NULL;
	cmd->path = NULL;
	cmd->envp = NULL;
}
