/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hassende <hassende@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/10 12:02:07 by hassende          #+#    #+#             */
/*   Updated: 2025/03/10 13:13:49 by hassende         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char*	get_home_var(char **envp)
{
	int		i;
	char	*rtn;

	i = -1;
	while (envp[++i])
		if (!ft_strncmp(envp[i], "HOME=", 5)) // should we handle if the evalutor doesn't have HOME? (with unset maybe)
			break ;
	if (!envp[i])
	{
		ft_putstr_fd("cd: HOME not set\n", STDERR_FILENO);
		return (NULL);
	}
	rtn = ft_strdup(envp[i] + 5);
	if (!rtn)
		return (NULL);
	return (rtn);
}

void	do_cd(t_cmd *cmd, t_cmd_path *path)
{
	char	*new_path;

	if (cmd->cmd_split[1] == NULL)
	{
		new_path = get_home_var(path->envp);
		if (!new_path)
			return ;
	}
	else if (cmd->cmd_split[2] != NULL)
	{
		ft_putstr_fd("cd: too many arguments\n", STDERR_FILENO);
		return ;
	}
	else
		new_path = cmd->cmd_split[1];
	if (chdir(new_path) == -1)
	{
		ft_putstr_fd("cd: ", STDERR_FILENO);
		ft_putstr_fd(new_path, STDERR_FILENO);
		ft_putstr_fd(": No such file or directiory\n", STDERR_FILENO);
		exit (1);
	}
}
