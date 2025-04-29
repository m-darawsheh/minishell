/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hassende <hassende@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/23 15:16:37 by hassende          #+#    #+#             */
/*   Updated: 2025/04/29 15:55:47 by hassende         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	heredoc_signal_handler(int sig)
{
	if (sig == SIGINT)
	{
		g_heredoc_interrupted = 1;
		write(1, "\n", 1);
		close(0);
	}
}

static void	handle_heredoc_interruption(t_cmd *cmd)
{
	int	new_stdin;
	int	arg;

	new_stdin = open("/dev/tty", O_RDONLY);
	if (new_stdin != -1)
	{
		dup2(new_stdin, STDIN_FILENO);
		arg = 0;
		ioctl(new_stdin, TCFLSH, &arg);
	}
	cmd->skip_exec = 1;
}

static void	process_heredoc_input(t_cmd *cmd, int fd_write, int j)
{
	char	*line;

	g_heredoc_interrupted = 0;
	signal(SIGINT, heredoc_signal_handler);
	line = readline("> ");
	while (line && ft_strncmp(line, cmd->delimiter[j], MAX_CMD_LEN) != 0)
	{
		ft_putendl_fd(line, fd_write);
		free(line);
		line = readline("> ");
	}
	if (!line && g_heredoc_interrupted)
		handle_heredoc_interruption(cmd);
	free(line);
}

void	handle_heredoc(t_cmd *cmd, int j)
{
	int	fd[2];

	if (pipe(fd) == -1)
		exit_error("Pipe failed");
	process_heredoc_input(cmd, fd[1], j);
	setup_signals();
	close(fd[1]);
	cmd->heredoc_fd = fd[0];
}
