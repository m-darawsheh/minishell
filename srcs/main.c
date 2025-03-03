/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hassende <hassende@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/25 14:25:54 by mdarawsh          #+#    #+#             */
/*   Updated: 2025/03/03 13:06:11 by hassende         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// void	execute_cmd(t_cmd *cmd)
// {
// 	int		i;
// 	char	*tmp;
// 	char	*exec_path;

// 	i = 0;
// 	cmd->cmd_split = ft_split(cmd->cmd, ' ');
// 	while (cmd->path[i])
// 	{
// 		tmp = ft_strjoin(cmd->path[i], "/");
// 		exec_path = ft_strjoin(tmp, cmd->cmd_split[0]);
// 		free(tmp);
// 		if (access(exec_path, X_OK) == 0)
// 		{
// 			execve(exec_path, cmd->cmd_split, cmd->envp);
// 			free(exec_path);
// 			exit(1);
// 		}
// 		free(exec_path);
// 		i++;
// 	}
// 	ft_putstr_fd(cmd->cmd, 2);
// 	ft_putstr_fd(": Command not found\n", 2);
// 	exit(1);
// }

int	main(int argc, char **argv, char **envp)
{
	(void)argv;
	char		*line_read;
	t_cmd_path	cmd_path;

	if (argc > 1)
		exit_error("Usage: ./minishell");
	struc_init(&cmd_path, envp);
	setup_signals();
	while (1)
	{
		line_read = readline("minishell$ ");
		if (!line_read)
			break ;
		if (*line_read)
			add_history(line_read);
		exec_cmd(line_read, &cmd_path);
		// exec_prepare(&cmd, line_read);
	}
}
