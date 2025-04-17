/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_handle.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hassende <hassende@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/09 23:43:49 by mdarawsh          #+#    #+#             */
/*   Updated: 2025/04/17 21:03:59 by hassende         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	add_env(t_cmd_path *path, t_cmd *cmd, int index)
{
	int		i;
	char	*tmp;

	i = 0;
	tmp = ft_strdup(cmd->cmd_split[index]);
	if (!tmp)
		return ;
	while (path->envp[i])
		i++;
	path->envp = realloc_2d(path->envp, i, i + 1);
	if (!path->envp)
	{
		free(tmp);
		return ;
	}
	path->envp[i] = ft_strdup(tmp);
	if (!path->envp[i])
	{
		free(tmp);
		return ;
	}
	path->envp[i + 1] = NULL;
	free(tmp);
}

int	check_env(t_cmd_path *path, t_cmd *cmd, int *i, int index)
{
	int	j;

	j = 0;
	while (cmd->cmd_split[index][j] != '=')
		j++;
	while (path->envp[*i])
	{
		if (ft_strncmp(path->envp[*i], cmd->cmd_split[index], j) == 0)
			return (*i);
		(*i)++;
	}
	return (0);
}

void	edit_env(t_cmd_path *path, t_cmd *cmd, int *i, int index)
{
	int	j;
	int	k;

	j = 0;
	k = 0;
	while (cmd->cmd_split[index][j] != '=')
		j++;
	while (path->envp[*i][k] != '=')
		k++;
	free(path->envp[*i]);
	path->envp[*i] = ft_strdup(cmd->cmd_split[index]);
}

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

int	valid_input(t_cmd *cmd, int index)
{
	int	i;

	i = 0;
	while (cmd->cmd_split[index][i])
	{
		while (cmd->cmd_split[index][i] && cmd->cmd_split[index][i] != '=')
		{
			if (!(ft_isalpha(cmd->cmd_split[index][i]) || ft_isdigit(cmd->cmd_split[index][i]) || cmd->cmd_split[index][i] == '_'))
			{
				printf("export: `%s': not a valid identifier\n", cmd->cmd_split[index]);
				return (0);
			}
			i++;
		}
		if (cmd->cmd_split[index][i])
		{
			i++;
			if (cmd->cmd_split[index][0] == '=')
				return (0);
			while (cmd->cmd_split[index][i])
			{
				printf("%c", cmd->cmd_split[index][i]);
				// if the value in quotes we need skip  the  spaces
				if (cmd->cmd_split[index][i] == '"' || cmd->cmd_split[index][i] == '\'')
				{
					printf("\n\n\n\n\ni inter\n\n\n\n\n\n");
					while (cmd->cmd_split[index][i] && cmd->cmd_split[index][i] != '"' && cmd->cmd_split[index][i] != '\'')
					{
						printf("export: `%s': not a valid identifier\n", cmd->cmd_split[index]);
						i++;
					}
					return (1);
				}
				if (!(ft_isalpha(cmd->cmd_split[index][i]) || ft_isdigit(cmd->cmd_split[index][i]) || cmd->cmd_split[index][i] == '_'))
					return (0);
				i++;
			}
			if (cmd->cmd_split[index][i - 1] == '=')
				return (0);
			return (1);
		}
		else
			return (0);
	}
	return (1);
}

void	export_handle(t_cmd *cmd, t_cmd_path *path)
{
	int	i;
	int	index;

	i = 0;
	index = 1;
	if (cmd->cmd_split[1] == NULL)
	{
		print_export(path);
		return;
	}
	while (cmd->cmd_split[index])
	{
		if (!valid_input(cmd, index))
		{
			printf("somthing wrong with must put valid input like this -> name=value\n");
			index++;
			continue ;
		}
		if (check_env(path, cmd, &i, index))
			edit_env(path, cmd, &i, index);
		else
			add_env(path, cmd, index);
		index++;
	}
}
