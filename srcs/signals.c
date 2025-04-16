/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hassende <hassende@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/26 15:17:16 by hassende          #+#    #+#             */
/*   Updated: 2025/04/16 19:37:21 by hassende         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

volatile sig_atomic_t g_heredoc_interrupted = 0;

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
