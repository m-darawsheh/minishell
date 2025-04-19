/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdarawsh <mdarawsh@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/20 14:28:49 by hassende          #+#    #+#             */
/*   Updated: 2025/04/19 11:23:39 by mdarawsh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	remove_quotes(t_token *token)
{
	char	new_value[ft_strlen(token->value) + 1];
	int		i;
	int		j;
	int		quote_state;

	i = 0;
	j = 0;
	quote_state = 0;

	while (token && token->value[i])
	{
		printf("remove_quotes the i size is %d\n" , i);
		if (token->value[i] == '\'' && quote_state == 0)
			quote_state = 1;
		else if (token->value[i] == '\'' && quote_state == 1)
			quote_state = 0;
		else if (token->value[i] == '\"' && quote_state == 0)
			quote_state = 2;
		else if (token->value[i] == '\"' && quote_state == 2)
			quote_state = 0;
		else
			new_value[j++] = token->value[i];
		i++;
	}
	new_value[j] = '\0';
	free(token->value);
	token->value = ft_strdup(new_value);
}

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
	char	var_name[MAX_ENV_NAME];
	int		j;
	int		start;

	start = i;
	i++;
	j = 0;
	if (token && token->value && token->value[i] == '?')
	{
		var_name[j++] = token->value[i++];
		var_name[j] = '\0';
		change_value(token, start, i, ft_itoa(path->exit_status));  // Default to 0 for now
	}
	else
	{
		while (token && token->value && token->value[i] && (ft_isalnum(token->value[i])
				|| token->value[i] == '_'))
			var_name[j++] = token->value[i++];
		var_name[j] = '\0';
		if (j > 0)
			change_value(token, start, i, get_env_value(var_name, path));
	}
}
int	count_tokens(t_token **tokens)
{
	int	i;

	i = 0;
	if (!tokens)
		return (0);
	while (tokens[i])
		i++;
	return (i);
}
char *join_all_tokens(t_token **tokens)
{
	int		i;
	char	*str;
	char	*temp;

	i = 0;
	str = ft_strdup("");
	if (!str)
		return (NULL);
	while (tokens[i])
	{
		temp = ft_strjoin(str, tokens[i]->value);
		free(str);
		if (!temp)
			return (NULL);
		if (count_tokens(tokens) == i + 1)
		{
			return (temp);
		}
		str = ft_strjoin(temp, " ");
		free(temp);
		if (!str)
			return (NULL);
		i++;
	}
	return (str);
}

void expand_2(t_token ***tokens,t_token *token, int index_token, int i,t_cmd_path *path)
{
	t_token **temp;

	temp = *tokens;
	token->was_in_double_quotes = 0;
	token->the_index_must_be_split_on_space = index_token;
	char *str;
	str = NULL;
	expand(token, i, path);
	str = join_all_tokens(*tokens);
	if (ft_strlen(str) == 0)
	{
		free(str);
		str = ft_strdup("\"\"");
	}
	printf("str: %s\n", str);
	free_tokens(*tokens);
	*tokens = tokenize(str,1);
	temp = *tokens;
	for(int k = 0; temp[k]; k++)
	{
		printf("token[%d]: %s\n", k, temp[k]->value);
	}
	free(str);
}

static void	check_for_expansion(t_token ***tokens, t_token *token, int index_token, t_cmd_path *path)
{
	int	i;
	int	quote_state;

	i = 0;
	quote_state = 0;
	while (token && token->value && token->value[i])
	{

		if (token->value[i] == '\'' && quote_state == 0)
			quote_state = 1;
		else if (token->value[i] == '\'' && quote_state == 1)
			quote_state = 0;
		else if (token->value[i] == '\"' && quote_state == 0)
			quote_state = 2;
		else if (token->value[i] == '\"' && quote_state == 2)
			quote_state = 0;
		else if (token->value[i] == '$' && quote_state != 1)
		{
			if (quote_state == 2)
			{
				token->was_in_double_quotes = 1;
				token->the_index_must_be_split_on_space = -1;
				expand(token, i, path);
			}
			else
				expand_2(tokens,token, index_token, i,path);
		}
		i++;
		printf("the value is %s\n", token->value);
	}
	remove_quotes(token);
}

void expander(t_token ***tokens, t_cmd_path *path)
{
	int i;

	i = -1;
	while (tokens[0][++i])
		if (tokens[0][i]->type == TOKEN_WORD)
			check_for_expansion(tokens, tokens[0][i], i, path);
}
