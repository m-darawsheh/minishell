/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hassende <hassende@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/25 14:25:54 by mdarawsh          #+#    #+#             */
/*   Updated: 2025/02/25 17:31:13 by hassende         ###   ########.fr       */
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

	i = 0;
	cmd->cmd_split = ft_split(cmd->cmd, ' ');
	while (cmd->path[i])
	{
		tmp = ft_strjoin(cmd->path[i], "/");
		tmp = ft_strjoin(tmp, cmd->cmd);
		if (access(tmp, X_OK) == 0)
			execve(tmp, cmd->cmd_split, cmd->envp);
		i++;
	}
}

int	main(int argc, char **argv, char **envp)
{
	(void)argv;
	t_cmd	cmd;
	pid_t	pid;

	cmd.envp = envp;
	if (argc > 1)
		exit_error("Usage: ./minishell");

	find_path(&cmd);
	cmd.cmd_split = NULL;
	while (1)
	{
		cmd.cmd = readline("minishell$ ");
		if (!cmd.cmd)
			break ;
		if (*cmd.cmd)
			add_history(cmd.cmd);
		pid = fork();
		if (!pid)
			execute_cmd(&cmd);
		wait (NULL);
	}
	free_all(&cmd);
}
