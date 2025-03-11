/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdarawsh <mdarawsh@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/10 12:02:07 by hassende          #+#    #+#             */
/*   Updated: 2025/03/12 01:28:49 by mdarawsh         ###   ########.fr       */
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
void	change_pwd(t_cmd_path *path)
{
	int		i;
	char	*tmp;
	char	*new_pwd;

	i = 0;
	while (path->envp[i] && ft_strncmp(path->envp[i], "PWD=", 4))
		i++;
	if (path->envp[i])
	{
		tmp = ft_strdup(path->envp[i] + 4);
		free(path->envp[i]);
		new_pwd = getcwd(NULL, 0);
		if (!new_pwd)
		{
			ft_putstr_fd("cd: error retrieving current directory\n", STDERR_FILENO);
			free(tmp);
			return ;
		}
		path->envp[i] = ft_strjoin("PWD=", new_pwd);
		free(new_pwd);
	}
	i = 0;
	while (path->envp[i] && ft_strncmp(path->envp[i], "OLDPWD=", 7))
		i++;
	if (path->envp[i])
	{
		free(path->envp[i]);
		path->envp[i] = ft_strjoin("OLDPWD=", tmp);
	}
	free(tmp);
}

// if new_path is set from the first (if) free it if not do nothing
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
	change_pwd(path);
}
