/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hassende <hassende@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/25 14:25:54 by mdarawsh          #+#    #+#             */
/*   Updated: 2025/02/26 15:49:05 by hassende         ###   ########.fr       */
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

void	stack_add_bottom(t_cmd **cmd, char *line_read)
{
	t_cmd	*new_cmd;
	t_cmd	*tmp;

	new_cmd = malloc(sizeof(t_cmd));
	if (!new_cmd)
		exit_error("Malloc failed");
	new_cmd->has_pipe = 0;
	new_cmd->has_infile = 0;
	new_cmd->has_outfile = 0;
	new_cmd->has_appendfile = 0;
	new_cmd->has_heredoc = 0;
	new_cmd->cmd = line_read;
	new_cmd->cmd_split = NULL;
	new_cmd->infile = NULL;
	new_cmd->outfile = NULL;
	new_cmd->cmd_path = NULL;
	new_cmd->next = NULL;
	tmp = *cmd;
	if (!tmp)
	{
		*cmd = new_cmd;
		return ;
	}
	while (tmp->next)
		tmp = tmp->next;
	tmp->next = new_cmd;
}

int	main(int argc, char **argv, char **envp)
{
	(void)argv;
	char		*line_read;
	t_cmd		*cmd;
	t_cmd_path	cmd_path;

	if (argc > 1)
		exit_error("Usage: ./minishell");
	cmd = malloc(sizeof(t_cmd));
	if (!cmd)
		exit_error("Malloc failed");
	struc_init(&cmd_path, envp);
	setup_signals();
	while (1)
	{
		line_read = readline("minishell$ ");
		if (!line_read)
			break ;
		if (*line_read)
			add_history(line_read);
		stack_add_bottom(&cmd, line_read);
		// exec_prepare(&cmd, line_read);
	}
}
