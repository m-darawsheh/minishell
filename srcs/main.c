/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hassende <hassende@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/25 14:25:54 by mdarawsh          #+#    #+#             */
/*   Updated: 2025/02/25 18:42:59 by hassende         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	find_path(t_cmd *cmd)
{
	int	i;

	i = 0;
	while (cmd->envp[i])
	{
		if (ft_strncmp(cmd->envp[i], "PATH=", 5) == 0)
		{
			cmd->path = ft_split(cmd->envp[i] + 5, ':');
			break ;
		}
		i++;
	}
}


void	execute_cmd(t_cmd *cmd)
{
	int		i;
	char	*tmp;
	char	*exec_path;

	i = 0;
	cmd->cmd_split = ft_split(cmd->cmd, ' ');
	while (cmd->path[i])
	{
		tmp = ft_strjoin(cmd->path[i], "/");
		exec_path = ft_strjoin(tmp, cmd->cmd_split[0]);
		free(tmp);
		if (access(exec_path, X_OK) == 0)
		{
			execve(exec_path, cmd->cmd_split, cmd->envp);
			free(exec_path);
			exit(1);
		}
		free(exec_path);
		i++;
	}
	ft_putstr_fd(cmd->cmd, 2);
	ft_putstr_fd(": Command not found\n", 2);
	exit(1);
}

void	sig_handler(int sig)
{
	if (sig == SIGINT)
	{
		ft_putstr_fd("\n", 1);
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
	}
}

void    setup_signals(void)
{
    signal(SIGINT, sig_handler);
    signal(SIGQUIT,  SIG_IGN);
}

int	main(int argc, char **argv, char **envp)
{
	(void)argv;
	t_cmd	cmd;

	struc_init(&cmd);
	cmd.envp = envp;
	if (argc > 1)
		exit_error("Usage: ./minishell");

	find_path(&cmd);
	setup_signals();
	while (1)
	{
		cmd.cmd = readline("minishell$ ");
		if (!cmd.cmd)
			break ;
		if (*cmd.cmd)
			add_history(cmd.cmd);
		cmd.pid = fork();
		if (cmd.pid == 0)
			execute_cmd(&cmd);
		wait (NULL);
	}
	free_all(&cmd);
}
