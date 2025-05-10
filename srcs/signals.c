/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hassende <hassende@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/26 15:17:16 by hassende          #+#    #+#             */
/*   Updated: 2025/05/10 13:30:52 by hassende         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

volatile sig_atomic_t	g_heredoc_interrupted = 0;

void	clean_main_cmd_fds(t_cmd *cmd)
{
	int	i;

	i = 0;
	while (cmd->main_cmd[i])
	{
		if (cmd->main_cmd[i] != cmd && cmd->main_cmd[i]->has_heredoc
			&& cmd->main_cmd[i]->heredoc_fd != -1)
			close(cmd->main_cmd[i]->heredoc_fd);
		i++;
	}
}

void	interactive_sig_handler(int sig)
{
	if (sig == SIGINT)
	{
		g_heredoc_interrupted = 130;
		ft_putstr_fd("\n", 1);
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
	}
}

void	setup_interactive_signals(void)
{
	signal(SIGINT, interactive_sig_handler);
	signal(SIGQUIT, SIG_IGN);
	signal(SIGTSTP, SIG_IGN);
}

void	setup_exec_signals(void)
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
}

void	setup_signals(void)
{
	setup_interactive_signals();
}
