/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_handle.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdarawsh <mdarawsh@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/09 23:43:49 by mdarawsh          #+#    #+#             */
/*   Updated: 2025/04/11 15:39:46 by mdarawsh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void add_env(t_cmd_path *path, t_token *token)
{
	int i;
	char *tmp;

	i = 0;
	tmp = ft_strdup(token->value);
	if (!tmp)
		return;
	while (path->envp[i])
		i++;
	path->envp = realloc_2d(path->envp, i, i + 1);
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
int check_env(t_cmd_path *path, int *i, t_token *token)
{
	int j;

	j = 0;
	while (token->value[j] != '=')
		j++;
	while (path->envp[*i])
	{
		if (ft_strncmp(path->envp[*i], token->value, j) == 0)
			return (*i);
		(*i)++;
	}
	return (0);
}

void edit_env(t_cmd_path *path, int *i, t_token *token)
{
	int j;
	int k;

	j = 0;
	k = 0;
	while (token->value[j] != '=')
		j++;
	while (path->envp[*i][k] != '=')
		k++;
	free(path->envp[*i]);
	path->envp[*i] = ft_strdup(token->value);
}

void print_export(t_cmd_path *path)
{
	int i;

	i = 0;
	while (path->envp[i])
	{
		printf("declare -x %s\n", path->envp[i]);
		i++;
	}
}

	// while (cmd->cmd_split[index][i])
	// {
	// 	while (cmd->cmd_split[index][i] && cmd->cmd_split[index][i] != '=')
	// 	{
	// 		if (!(ft_isalpha(cmd->cmd_split[index][i]) || ft_isdigit(cmd->cmd_split[index][i]) || cmd->cmd_split[index][i] == '_'))
	// 		{
	// 			printf("export: `%s': not a valid identifier\n", cmd->cmd_split[index]);
	// 			return (0);
	// 		}
	// 		i++;
	// 	}

	// 	printf("dsfaasf   %c", cmd->cmd_split[index][i]);
	// 	if (cmd->cmd_split[index][i])
	// 	{
	// 		i++;
	// 	printf("fdsa    %c", cmd->cmd_split[index][i]);

	// 		if (cmd->cmd_split[index][0] == '=')
	// 			return (0);
	// 		while (cmd->cmd_split[index][i])
	// 		{
	// 			// if the value in quotes we need skip  the  spaces
	// 			if (!(ft_isalpha(cmd->cmd_split[index][i]) || ft_isdigit(cmd->cmd_split[index][i]) || cmd->cmd_split[index][i] == '_'))
	// 			{

	// 				return (0);
	// 			}
	// 			i++;
	// 		}
	// 		if (cmd->cmd_split[index][i - 1] == '=')
	// 			return (0);
	// 		return (1);
	// 	}
	// 	else
	// 		return (0);
	// }

int	there_is_equal(t_token *tokens)
{
	int i;

	i = 0;
	while ( tokens->value[i])
	{
		if (tokens->value[i] == '=')
			return (i);
		i++;
	}
	return (0);
}

int valid_input(int index, t_token **tokens)
{
	int	j;

	j = there_is_equal(tokens[index]);
	// printf("index: %d\n", index);
	// printf("j: %d\n", j);
	if (j)
	{
		if (tokens[index]->value[j + 1] == '\0' && tokens[index]->value[0] != '=')
		{
			return (1);
		}
		else if (ft_isalpha(tokens[index]->value[j + 1]) || ft_isdigit(tokens[index]->value[j + 1]) || tokens[index]->value[j + 1] == '=')
		{
			return (1);
		}
		else if (ft_isalpha(tokens[index]->value[j - 1]) || ft_isdigit(tokens[index]->value[j - 1]) || tokens[index]->value[j - 1] == '=')
		{
			return (1);
		}
		else
			return (0);
	}
	else
		return (0);
	return (0);
}

void export_handle(t_cmd *cmd, t_cmd_path *path, t_token **tokens)
{
	int i;
	int index;

	i = 0;
	index = 1;
	int token_index = 0;
	while (tokens[token_index])
	{
		printf("token[%d]: %s\n", token_index, tokens[token_index]->value);
		token_index++;
	}
	if (cmd->cmd_split[1] == NULL)
	{
		print_export(path);
		return;
	}
	while (tokens[index])
	{
		if (!valid_input(index, tokens))
		{
			printf("somthing wrong with must put valid input like this -> name=value %s\n" , tokens[index]->value);
			index++;
			continue;
		}
		if (check_env(path, &i, tokens[index]))
			edit_env(path, &i, tokens[index]);
		else
			add_env(path, tokens[index]);
		index++;
	}
}
