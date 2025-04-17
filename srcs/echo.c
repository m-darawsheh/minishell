/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hassende <hassende@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 19:24:24 by hassende          #+#    #+#             */
/*   Updated: 2025/04/17 19:25:34 by hassende         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	check_all_n(char *str)
{
	int	i;

	i = 1;
	while (str[i] == 'n')
		i++;
	if (str[i] == '\0')
		return (1);
	return (0);
}

void	do_echo(t_cmd *cmd)
{
	int	i;
	int	n_flag;

	i = 1;
	n_flag = 0;
	while (cmd->cmd_split[i] && cmd->cmd_split[i][0] == '-' &&
			cmd->cmd_split[i][1] == 'n' && check_all_n(cmd->cmd_split[i]))
	{
		n_flag = 1;
		i++;
	}
	while (cmd->cmd_split[i])
	{
		ft_putstr_fd(cmd->cmd_split[i], STDOUT_FILENO);
		if (cmd->cmd_split[i + 1])
			ft_putchar_fd(' ', STDOUT_FILENO);
		i++;
	}
	if (!n_flag)
		ft_putchar_fd('\n', STDOUT_FILENO);
}
