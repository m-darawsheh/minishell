/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hassende <hassende@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/25 14:25:54 by mdarawsh          #+#    #+#             */
/*   Updated: 2025/03/03 14:40:31 by hassende         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	main(int argc, char **argv, char **envp)
{
	(void)argv;
	char		*line_read;
	t_cmd_path	path;

	if (argc > 1)
		exit_error("Usage: ./minishell");
	struc_init(&path, envp);
	setup_signals();
	while (1)
	{
		line_read = readline("minishell$ ");
		if (!line_read)
			break ;
		if (*line_read)
			add_history(line_read);
		exec_cmd(line_read, &path);
	}
}
