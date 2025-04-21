/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hassende <hassende@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/25 14:25:54 by mdarawsh          #+#    #+#             */
/*   Updated: 2025/04/21 21:50:42 by hassende         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	check_if_empty(char *line_read)
{
	int	i;

	i = 0;
	while (line_read[i])
	{
		if (line_read[i] != ' ' && line_read[i] != '\t')
			return (0);
		i++;
	}
	return (1);
}

void	process_command_line(char *line_read, t_cmd_path *path)
{
	if (g_heredoc_interrupted == 130)
	{
		path->exit_status = 130;
		g_heredoc_interrupted = 0;
	}
	if (*line_read == '\0' || check_if_empty(line_read))
	{
		free(line_read);
		return ;
	}
	if (*line_read)
		add_history(line_read);
	exec_cmd(line_read, path);
	free(line_read);
}

int	main(int argc, char **argv, char **envp)
{
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
		process_command_line(line_read, &path);
	}
	free_2d(path.envp);
	free_2d(path.path);
	rl_clear_history();
	(void)argv;
}
