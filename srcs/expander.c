/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hassende <hassende@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/20 14:28:49 by hassende          #+#    #+#             */
/*   Updated: 2025/05/10 13:57:38 by hassende         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*get_env_value(const char *name, t_cmd_path *path)
{
	int		i;
	int		name_len;
	char	*env_var;

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
	new_value = malloc(new_len + 1);
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

void	expand(t_token *token, int i, t_cmd_path *path)
{
	char	var_name[MAX_ENV_NAME];
	int		j;
	int		start;

	start = i;
	i++;
	j = 0;
	if (token->value[i] == '?')
		change_value(token, start, i + 1, ft_itoa(path->exit_status));
	else if (token->value[i] == '0')
		change_value(token, start, i + 1, ft_strdup("minishell"));
	else if (ft_isdigit(token->value[i]))
		change_value(token, start, i + 1, ft_strdup(""));
	else
	{
		while (token->value[i] && (ft_isalnum(token->value[i])
				|| token->value[i] == '_'))
			var_name[j++] = token->value[i++];
		var_name[j] = '\0';
		if (j > 0)
			change_value(token, start, i, get_env_value(var_name, path));
	}
}

static void	set_quote_state(int *quote_state, char c, t_token *token)
{
	if (c == '\'' && *quote_state == 0)
		*quote_state = 1;
	else if (c == '\'' && *quote_state == 1)
		*quote_state = 0;
	else if (c == '\"' && *quote_state == 0)
	{
		token->quoted = 1;
		*quote_state = 2;
	}
	else if (c == '\"' && *quote_state == 2)
		*quote_state = 0;
}

void	check_for_expansion(t_token *token, t_cmd_path *path)
{
	int	i;
	int	quote_state;

	if (token->value == NULL || !token->value[0])
		return ;
	i = 0;
	quote_state = 0;
	while (token->value[i])
	{
		set_quote_state(&quote_state, token->value[i], token);
		if (token->value[i] == '$' && quote_state != 1)
			process_dollar_sign(token, &i, path);
		i++;
	}
	remove_quotes(token);
}
