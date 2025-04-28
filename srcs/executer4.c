/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executer4.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hassende <hassende@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/28 17:19:46 by hassende          #+#    #+#             */
/*   Updated: 2025/04/28 17:39:55 by hassende         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	append_args(char ***new_args, t_cmd *cmd)
{
	int	i;
	int	j;

	i = 0;
	while ((*new_args)[i])
		i++;
	j = 1;
	while (cmd->cmd_split[j])
		j++;
	*new_args = realloc_2d(*new_args, i, i + j);
	if (!*new_args)
		return ;
	j = 1;
	while (cmd->cmd_split[j])
	{
		(*new_args)[i] = ft_strdup(cmd->cmd_split[j]);
		if (!(*new_args)[i])
			return ;
		i++;
		j++;
	}
	(*new_args)[i] = NULL;
}

void	restore_io(int stdin_backup, int stdout_backup)
{
	if (stdin_backup != -1)
	{
		dup2(stdin_backup, STDIN_FILENO);
		close(stdin_backup);
	}
	if (stdout_backup != -1)
	{
		dup2(stdout_backup, STDOUT_FILENO);
		close(stdout_backup);
	}
}
