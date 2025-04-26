/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdarawsh <mdarawsh@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 20:47:36 by hassende          #+#    #+#             */
/*   Updated: 2025/04/26 17:40:56 by mdarawsh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	check_overflow(const char *str)
{
	int					i;
	unsigned long long	max;
	unsigned long long	result;

	i = 0;
	max = 9223372036854775807;
	result = 0;
	if (str[i] == '-')
	{
		max++;
		i++;
	}
	else if (str[i] == '+')
		i++;
	while (str[i] >= '0' && str[i] <= '9')
	{
		if (result > max / 10 || (result == max / 10
				&& (unsigned long long)(str[i] - '0') > max % 10))
			return (1);
		result = result * 10 + (str[i] - '0');
		i++;
	}
	return (0);
}

static int	exit_isdigit(char *str)
{
	int	i;

	i = 0;
	if (str[i] == '-')
		i++;
	while (ft_isdigit(str[i]))
		i++;
	if (i != ft_strlen(str))
		return (0);
	return (1);
}

int	do_exit(t_cmd *cmd)
{
	long long	exit_code;

	if (cmd->cmd_split[1] == NULL)
		exit(0);
	if (cmd->cmd_split[2] != NULL)
	{
		ft_putstr_fd("minishell: exit: too many arguments\n", 2);
		return (1);
	}
	if (!exit_isdigit(cmd->cmd_split[1]) || check_overflow(cmd->cmd_split[1]))
	{
		ft_putstr_fd("minishell: exit: ", 2);
		ft_putstr_fd(cmd->cmd_split[1], 2);
		ft_putstr_fd(": numeric argument required\n", 2);
		exit (2);
	}
	exit_code = ft_atoi(cmd->cmd_split[1]);
	exit_code %= 256;
	exit ((int)exit_code);
}

void	free_tokens(t_token **tokens)
{
	int	i;

	i = -1;
	if (!tokens)
		return ;
	while (tokens[++i])
	{
		free(tokens[i]->value);
		free(tokens[i]);
	}
	free(tokens);
}
