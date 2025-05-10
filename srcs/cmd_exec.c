/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_exec.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hassende <hassende@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/03 12:48:35 by hassende          #+#    #+#             */
/*   Updated: 2025/05/10 13:53:45 by hassende         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int		process_heredocs(t_cmd **cmd);
static t_cmd	**parse_and_prepare(char *line_read, t_cmd_path *path,
					t_token ***tokens_head);

void	close_fds(t_cmd **cmd)
{
	int	i;

	i = 0;
	while (cmd[i])
	{
		if (cmd[i]->has_heredoc)
			close(cmd[i]->heredoc_fd);
		i++;
	}
}

void	exec_cmd(char *line_read, t_cmd_path *path)
{
	t_cmd	**cmd;
	t_token	**tokens;

	cmd = parse_and_prepare(line_read, path, &tokens);
	if (!cmd)
	{
		free_cmds(cmd, 0);
		return ;
	}
	if (!process_heredocs(cmd))
	{
		path->exit_status = 130;
		free_tokens(tokens);
		return ;
	}
	prepare_command_splits(cmd, tokens);
	free_tokens(tokens);
	execute_command(cmd, path);
	close_fds(cmd);
	free_cmds(cmd, 0);
}

static t_cmd	**parse_and_prepare(char *line_read, t_cmd_path *path,
					t_token ***tokens_head)
{
	t_cmd	**cmd;
	t_token	**tokens;

	cmd = t_cmd_malloc(line_read, path);
	if (!cmd)
		return (NULL);
	tokens = tokenize(line_read, path);
	if (!tokens)
	{
		free_cmds(cmd, 0);
		return (NULL);
	}
	expander(tokens, path);
	if (!parse_token(tokens, cmd) || (!cmd[1] && cmd[0]->cmd[0] == '\0'))
	{
		free_cmds(cmd, 0);
		free_tokens(tokens);
		return (NULL);
	}
	*tokens_head = tokens;
	return (cmd);
}

static int	process_heredocs(t_cmd **cmd)
{
	int	i;
	int	j;

	i = -1;
	while (cmd[++i])
	{
		j = 0;
		if (cmd[i]->has_heredoc)
		{
			while (cmd[i]->delimiter[j] && j <= cmd[i]->heredoc_index
				&& !(cmd[i]->skip_exec))
			{
				if (cmd[i]->heredoc_fd != -1)
				{
					close(cmd[i]->heredoc_fd);
					cmd[i]->heredoc_fd = -1;
				}
				handle_heredoc(cmd[i], j);
				j++;
			}
		}
		if (cmd[i]->skip_exec)
			return (close_free(cmd[i]->heredoc_fd, cmd));
	}
	return (1);
}
