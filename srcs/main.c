/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hassende <hassende@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/25 14:25:54 by mdarawsh          #+#    #+#             */
/*   Updated: 2025/03/20 15:50:37 by hassende         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int ft_is_space(char c)
{
	return (c == ' ' || c == '\t' || c == '\n' || c == '\v' || c == '\f' || c == '\r');
}

int empty_input(char *line_read)
{
	int	i;

	i = 0;
	while (line_read[i])
	{
		if (!ft_is_space(line_read[i]))
			return (0);
		i++;
	}
	return (1);
}

int	main(int argc, char **argv, char **envp)
{
	char		*line_read;
	t_cmd_path	path;
	(void)argv;

	if (argc > 1)
		exit_error("Usage: ./minishell");
	struc_init(&path, envp);
	setup_signals();
	while (1)
	{
		line_read = readline("minishell$ ");
		if (!line_read)
			break ;
		if (empty_input(line_read))
		{
			free(line_read);
			continue ;
		}
		if (*line_read)
			add_history(line_read);
		exec_cmd(line_read, &path);
		free(line_read);
	}
	free_2d(path.envp);
	free_2d(path.path);
}
