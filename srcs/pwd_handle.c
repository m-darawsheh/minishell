/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd_handle.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hassende <hassende@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/09 03:55:12 by mdarawsh          #+#    #+#             */
/*   Updated: 2025/04/06 15:47:58 by hassende         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*find_pwd(t_cmd_path *path)
{
	char	*pwd;
	int		i;

	i = 0;
	while (path->envp[i])
	{
		if (ft_strncmp(path->envp[i], "PWD=", 4) == 0)
		{
			pwd = ft_strdup(path->envp[i] + 4);
			return (pwd);
		}
		i++;
	}
	return (NULL);
}

void	pwd_handle(t_cmd_path *path)
{
	char	*pwd;

	pwd = find_pwd(path);
	for (int i = 0; path->envp[i]; i++)
	{
		printf("%s\n", path->envp[i]);
	}
	if (!pwd)
	{
		perror("PWD not found");
		return;
	}
	printf("%s\n", pwd);
	free(pwd);
}
