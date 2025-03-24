/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdarawsh <mdarawsh@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 11:57:27 by mdarawsh          #+#    #+#             */
/*   Updated: 2025/03/24 13:50:41 by mdarawsh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*find_the_key(char *key, t_cmd_path *path)
{
	int	i;

	i = -1;
	while (*key == '$')
		key++;

	while (path->envp[++i])
	{
		if (ft_strncmp(key, path->envp[i], ft_strlen(key)) == 0)
		{
			return (path->envp[i]);
		}
	}
	return (NULL);

}

void	expander_switch(char *key, t_cmd_path *path)
{
	char	*str;

	str = find_the_key(key, path);
	if (str)
	{
		while (*str != '=')
			str++;
		str++;
	}
	if (str)
	{
		free(key);
		key = ft_strdup(str);
	}
	// else
	// {
	// 	free(key);
	// 	key = ft_strdup("");
	// }

}
void	handle_expander(t_cmd_path *path, t_token **tokens)
{
	int	i;
	int	j;

	i = -1;
	while (tokens[++i])
	{
		if (tokens[i]->type == TOKEN_WORD)
		{
			j = -1;
			while (tokens[i]->value[++j])
			{
				if (tokens[i]->value[j] == '$')
				{
					if (tokens[i]->value[j + 1] == '?')
					{
						continue;
					}
					else
					{
						expander_switch(tokens[i]->value, path);
						return ;
					}
				}
			}
		}
	}
}
