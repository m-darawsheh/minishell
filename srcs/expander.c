/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hassende <hassende@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/20 14:28:49 by hassende          #+#    #+#             */
/*   Updated: 2025/03/24 13:44:46 by hassende         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*get_env_value(const char *name, t_cmd_path *path)
{
	int		i;
	int		name_len;
	char	*env_var;

	// if (ft_strcmp(name, "?") == 0)
	// 	return ft_itoa(path->exit_status);
	// Search in environment variables
	i = 0;
	name_len = ft_strlen(name);
	while (path->envp[i])
	{
		if (ft_strncmp(path->envp[i], name, name_len) == 0
			&& path->envp[i][name_len] == '=')
		{
			env_var = ft_strdup(path->envp[i] + name_len + 1);
			return (env_var);
		}
		i++;
	}
	return (ft_strdup(""));
}

static void	change_value(t_token *token, int start, int end, char *env_var)
{
	char	*new_value;
	int		new_len;
	int		old_len;
	int		env_len;

	if (!env_var)
		return ;
	old_len = ft_strlen(token->value);
	env_len = ft_strlen(env_var);
	new_len = old_len - (end - start) + env_len;
	new_value = (char *)malloc(new_len + 1);
	if (!new_value)
		return ;
	ft_strlcpy(new_value, token->value, start + 1);
	ft_strlcpy(new_value + start, env_var, env_len + 1);
	ft_strlcpy(new_value + start + env_len,
		token->value + end,
		old_len - end + 1);
	free(token->value);
	token->value = new_value;
	free(env_var);
}

static void	expand(t_token *token, int i, t_cmd_path *path)
{
	char	tmp_buffer[MAX_ENV_NAME];
	int		j;
	int		tmp;

	tmp = i;
	j = 0;
	i++;
	while (token->value[i] != '\0' && !ft_strchr(" \t$", token->value[i]))
		tmp_buffer[j++] = token->value[i++];
	tmp_buffer[j] = '\0';
	change_value(token, tmp, i, get_env_value(tmp_buffer, path));
}

static void	check_for_expansion(t_token *token, t_cmd_path *path)
{
	int	i;
	int	in_quote;

	i = 0;
	in_quote = 0;
	if (token->value[i] == '\'')
	{
		in_quote = 1;
		i++;
	}
	if (token->value[i] == '\"')
	{
		i++;
		in_quote = 2;
	}
	// if (in_quote)
	// 	remove_quotes(token);
	while (token->value[i])
	{
		if (token->value[i] == '$' && (in_quote == 0 || in_quote == 2))
			expand(token, i, path);
		i++;
	}
}

void	expander(t_token **tokens, t_cmd_path *path)
{
	int	i;

	i = -1;
	while (tokens[++i] && tokens[i]->type == TOKEN_WORD)
		check_for_expansion(tokens[i], path);
}
