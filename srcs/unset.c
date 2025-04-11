/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdarawsh <mdarawsh@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/22 10:53:56 by mdarawsh          #+#    #+#             */
/*   Updated: 2025/04/11 18:01:23 by mdarawsh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	shift_envp(t_cmd_path *path, int index)
{
	int i;

	i = 0;
	while (path->envp[i])
		i++;
	while ()
	{
		
	}

}


void	handle_unset(t_cmd_path *path, t_token **tokens)
{
	int	i;
	int	j;

	j = 0;
	if (tokens[1]->value == NULL)
	{
		printf("minishell: unset: not enough arguments\n");
		return ;
	}
	else
	{
		while (tokens[j])
		{
			i = 0;
			while(path->envp[i])
			{
				if (ft_strncmp(tokens[j]->value, path->envp[i], ft_strlen(tokens[j]->value)) == 0)
				{
					printf("sadda\n");
					free(path->envp[i]);
					path->envp[i] = NULL;
					break;
				}
				i++;
			}
			j++;
		}
	}
	return ;
}
