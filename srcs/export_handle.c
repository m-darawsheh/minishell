/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_handle.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdarawsh <mdarawsh@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/09 23:43:49 by mdarawsh          #+#    #+#             */
/*   Updated: 2025/03/20 15:43:30 by mdarawsh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "minishell.h"



void	add_env(t_cmd_path *path , t_cmd *cmd)
{
	int		i;
	char	*tmp;

	i = 0;
	tmp = ft_strdup(cmd->cmd_split[1]);
	if (!tmp)
		return;
	while (path->envp[i])
		i++;
	path->envp = realloc_2d(path->envp, i , i + 1);
	if (!path->envp)
	{
		free(tmp);
		return;
	}
	path->envp[i] = ft_strdup(tmp);
	if (!path->envp[i])
	{
		free(tmp);
		return;
	}
	path->envp[i + 1] = NULL;
	free(tmp);
}
// export [0]
// VAR=newVALUE [1]
// VAR=oldValue
int check_env(t_cmd_path *path , t_cmd *cmd, int *i)
{
	int j;

	j = 0;

	while (cmd->cmd_split[1][j] != '=') // you should handle where the user doesn't put an equal "=" sign
		j++;
	while (path->envp[*i])
	{
		if (ft_strncmp(path->envp[*i], cmd->cmd_split[1], j) == 0)
			return (*i);
		(*i)++;
	}
	return (0);
}

int	there_is_equal(t_cmd *cmd)
{
	int i;

	i = 0;
	while (cmd->cmd_split[1][i])
	{
		if (cmd->cmd_split[1][i] == '=')
			return (1);
		i++;
	}
	return (0);
}

void	append_var(t_cmd_path *path , t_cmd *cmd)
{
	int i;

	i = 0;
	if (there_is_equal(cmd))
	{

	}
	else
	{
		
	}
}


void	edit_env(t_cmd_path *path , t_cmd *cmd, int *i)
{
	int j;
	int k;

	j = 0;
	k = 0;
	while (cmd->cmd_split[1][j] != '=')
		j++;
	while (path->envp[*i][k] != '=')
		k++;
	free(path->envp[*i]);
	path->envp[*i] = ft_strdup(cmd->cmd_split[1]);
}

// export test= fasfdsh
// test=space

// export test= 3
// error because of space

void	print_export(t_cmd_path *path)
{
	int	i;

	i = 0;
	while (path->envp[i])
	{
		printf("declare -x %s\n", path->envp[i]);
		i++;
	}
}


void	export_handle( t_cmd *cmd ,t_cmd_path *path)
{
	int i;

	i = 0;
	if (cmd->cmd_split[1] == NULL)
	{
		print_export(path);
		return ;
	}
	if (cmd->cmd_split[2] != NULL)
	{
		printf("minishell: export: %s: not a valid identifier\n", cmd->cmd_split[2]);
		return ;
	}
	if (check_env(path, cmd, &i))
		edit_env(path, cmd, &i);
	else
		add_env(path, cmd);
}
