/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdarawsh <mdarawsh@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/22 10:53:56 by mdarawsh          #+#    #+#             */
/*   Updated: 2025/04/14 14:47:47 by mdarawsh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	shift_envp(t_cmd_path *path, int index)
{
	free(path->envp[index]);
	path->envp[index] = NULL;
	while (path->envp[index + 1])
	{
		path->envp[index] = path->envp[index + 1];
		index++;
	}
	path->envp[index] = NULL;

}

// echo $USER
// echo "$USER"
// echo '$USER'
// echo "'$USER'"
// echo '"$USER"'
// echo ~
// echo $EMPTY
// export test=" echo 1 2        3 | ls "
// $test
// "$test"
// echo $test
// echo "$test"
// echo $test+$EMPTY+$USER
// echo $1USER
// echo $1USER$PWD$!
// echo $USER1
// echo $USER_



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
					shift_envp(path, i);
					break;
				}
				i++;
			}
			j++;
		}
	}
	return ;
}
