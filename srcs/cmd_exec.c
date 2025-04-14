/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_exec.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdarawsh <mdarawsh@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/03 12:48:35 by hassende          #+#    #+#             */
/*   Updated: 2025/04/14 16:22:52 by mdarawsh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static t_cmd **parse_and_prepare(char *line_read, t_cmd_path *path, t_token ***tokens);
static void process_heredocs(t_cmd **cmd);
static void prepare_command_splits(t_cmd **cmd, t_token **tokens);
static void execute_builtin(t_cmd *cmd, t_cmd_path *path, int stdin_backup, int stdout_backup, t_token **tokens);
static void setup_io_redirections(t_cmd *cmd);
static void setup_io_redirections_child(t_cmd *cmd, int *pipe_fd, int *prev_pipe, int i);
static void execute_builtin_child(t_cmd *cmd, t_cmd_path *path, t_token **tokens);
static void execute_command(t_cmd **cmd, t_cmd_path *path, t_token **tokens);
static void handle_pipes(int *pipe_fd, int *prev_pipe, t_cmd **cmd, int i);
static void wait_for_children(t_cmd_path *path);

void	print_env(t_cmd_path *path)
{
	int	i;

	i = 0;
	while (path->envp[i])
	{
		printf("%s\n", path->envp[i]);
		i++;
	}
}
void exec_cmd(char *line_read, t_cmd_path *path)
{
	t_cmd **cmd;
	t_token	**tokens;

	tokens = tokenize(line_read);

	cmd = parse_and_prepare(line_read, path, &tokens);
	if (!cmd)
		return;
	process_heredocs(cmd);
	prepare_command_splits(cmd, tokens);
	execute_command(cmd, path, tokens);

	free_cmds(cmd);
}




static t_cmd	**parse_and_prepare(char *line_read, t_cmd_path *path, t_token ***tokens)
{
	t_cmd	**cmd;

	cmd = t_cmd_malloc(line_read, path);
	if (!cmd)
		return (NULL);
	expander(tokens, path);
	// if (!)
	parse_token(tokens, cmd);
	// {
		// free_tokens(tokens);
		// return (NULL);
	// }
	// free_tokens(tokens);
	return (cmd);
}

static void process_heredocs(t_cmd **cmd)
{
	int i;

	i = -1;
	while (cmd[++i])
	{
		if (cmd[i]->has_heredoc)
			handle_heredoc(cmd[i]);

		if (cmd[i]->skip_exec)
		{
			free_cmds(cmd);
			return;
		}
	}
}

char **ft_realloc(char **str, char *new_str, int old_size)
{
	int i = 0;
	char **new;

	new = malloc((old_size + 2) * sizeof(char *));
	if (!new)
	{
		free(str);
		return (NULL);
	}
	while (i < old_size)
	{
		new[i] = ft_strdup(str[i]);
		if (!new[i])
		{
			while (i-- > 0)
				free(new[i]);
			free(new);
			free(str);
			return (NULL);
		}
		i++;
	}
	new[i++] = ft_strdup(new_str); // Duplicate it safely
	new[i] = NULL;
	free(str);
	return (new);
}

// void  split_for_expand2(t_cmd **cmd, t_token **tokens)
// {
// 	int	i;

// 	i = 0;
// 	while (tokens[i])
// 	{
// 		if (tokens[i]->type == TOKEN_WORD && !(tokens[i]->was_in_double_quotes) && tokens[i]->the_index_must_be_split_on_space != -1)
// 		{

// 		}
// 		i++;
// 	}


// }




static void prepare_command_splits(t_cmd **cmd, t_token **tokens)
{
	int i = -1;
	int j = 0;
	int count;


	// for(int k = 0; tokens[k]; k++)
	// {
	// 	printf("tokens[%d]: %s\n", k, tokens[k]->value);
	// }


	while (cmd[++i])
	{
		count = 0;
		while (tokens[j] && tokens[j]->type != TOKEN_PIPE)
		{

			if (tokens[j]->type == TOKEN_WORD)
			{
				if (count == 0 || (tokens[j - 1]->type != TOKEN_APPEND && tokens[j - 1]->type != TOKEN_HEREDOC && tokens[j - 1]->type != TOKEN_REDIR_IN && tokens[j - 1]->type != TOKEN_REDIR_OUT))
					cmd[i]->cmd_split = ft_realloc(cmd[i]->cmd_split, tokens[j]->value, count);
				count++;
			}
			// if (tokens[j]-> type == TOKEN_WORD && !(tokens[j]->was_in_double_quotes) && tokens[j]->the_index_must_be_split_on_space != -1)
			// {
			// 	// split_for_expand2
			// }
			j++;
		}
		j++;
	}
}

static void execute_builtin(t_cmd *cmd, t_cmd_path *path,
							int stdin_backup, int stdout_backup, t_token **tokens)
{
	if (!ft_strncmp(cmd->cmd_split[0], "echo", 4))
	{
		path->exit_status = 0;
		do_echo(cmd, tokens);
	}
	else if (!ft_strncmp(cmd->cmd_split[0], "exit", 4))
	{
		path->exit_status = do_exit(cmd);
	}
	else if (!ft_strncmp(cmd->cmd_split[0], "cd", 2))
	{
		path->exit_status = do_cd(cmd, path);
	}
	else if (!ft_strncmp(cmd->cmd_split[0], "export", 6))
	{
		path->exit_status = 0;
		export_handle(cmd, path, tokens);
	}
	else if (!ft_strncmp(cmd->cmd_split[0], "env", 3))
	{
		path->exit_status = 0;
		print_env(path);
	}
	else if (!ft_strncmp(cmd->cmd_split[0], "pwd", 3))
	{
		path->exit_status = 0;
		pwd_handle(path);
	}
	else if (!ft_strncmp(tokens[0]->value, "unset", 5))
	{
		path->exit_status = 0;
		handle_unset(path, tokens);
	}
	if (stdin_backup != -1)
	{
		dup2(stdin_backup, STDIN_FILENO);
		close(stdin_backup);
	}
	if (stdout_backup != -1)
	{
		dup2(stdout_backup, STDOUT_FILENO);
		close(stdout_backup);
	}
}

static void	setup_io_redirections(t_cmd *cmd)
{
	int	fd;

	if (cmd->has_infile)
	{
		fd = open(cmd->infile, O_RDONLY);
		if (fd == -1)
			exit_error("File not found");
		dup2(fd, STDIN_FILENO);
		close(fd);
	}
	if (cmd->has_outfile)
	{
		fd = open(cmd->outfile, O_CREAT | O_WRONLY | O_TRUNC, 0644);
		if (fd == -1)
			exit_error("File not found");
		dup2(fd, STDOUT_FILENO);
		close(fd);
	}
	if (cmd->has_appendfile)
	{
		fd = open(cmd->outfile, O_CREAT | O_WRONLY | O_APPEND, 0644);
		if (fd == -1)
			exit_error("File not found");
		dup2(fd, STDOUT_FILENO);
		close(fd);
	}
	if (cmd->has_heredoc)
	{
		dup2(cmd->heredoc_fd, STDIN_FILENO);
		close(cmd->heredoc_fd);
	}
}

static void setup_io_redirections_child(t_cmd *cmd, int *pipe_fd,
									   int *prev_pipe, int i)
{
	int	fd;

	if (cmd->has_heredoc)
	{
		dup2(cmd->heredoc_fd, STDIN_FILENO);
		close(cmd->heredoc_fd);
	}
	if (cmd->has_pipe)
	{
		dup2(pipe_fd[1], STDOUT_FILENO);
		close(pipe_fd[0]);
		close(pipe_fd[1]);
	}
	if (i > 0 && prev_pipe[0] != -1)
	{
		dup2(prev_pipe[0], STDIN_FILENO);
		close(prev_pipe[0]);
		close(prev_pipe[1]);
	}
	if (cmd->has_infile)
	{
		fd = open(cmd->infile, O_RDONLY);
		if (fd == -1)
			exit_error("File not found");
		dup2(fd, STDIN_FILENO);
		close(fd);
	}
	if (cmd->has_outfile)
	{
		fd = open(cmd->outfile, O_WRONLY | O_CREAT | O_TRUNC, 0644);
		if (fd == -1)
			exit_error("File not found");
		dup2(fd, STDOUT_FILENO);
		close(fd);
	}
	if (cmd->has_appendfile)
	{
		fd = open(cmd->outfile, O_WRONLY | O_CREAT | O_APPEND, 0644);
		if (fd == -1)
			exit_error("File not found");
		dup2(fd, STDOUT_FILENO);
		close(fd);
	}
}

static void execute_builtin_child(t_cmd *cmd, t_cmd_path *path, t_token **tokens)
{
	if (!ft_strncmp(cmd->cmd_split[0], "echo", 4))
	{
		do_echo(cmd, tokens);
		exit(0);
	}
	else if (!ft_strncmp(cmd->cmd_split[0], "exit", 4))
	{
		do_exit(cmd);
		exit(1);
	}
	else if (!ft_strncmp(cmd->cmd_split[0], "cd", 2))
	{
		int rtn_code = do_cd(cmd, path);
		if (rtn_code)
			exit(rtn_code);
		exit(0);
	}
	else if (!ft_strncmp(cmd->cmd_split[0], "export", 6))
	{
		export_handle(cmd, path, tokens);
		exit(0);
	}
	else if (!ft_strncmp(cmd->cmd_split[0], "env", 3))
	{
		print_env(path);
		exit(0);
	}
	setup_command(cmd, path);
	execve(cmd->cmd_path, cmd->cmd_split, path->envp);
	exit(127);
}

static void handle_pipes(int *pipe_fd, int *prev_pipe, t_cmd **cmd, int i)
{
	if (i > 0 && cmd[i-1]->has_pipe)
	{
		close(prev_pipe[0]);
		close(prev_pipe[1]);
	}
	if (cmd[i]->has_pipe)
	{
		if (cmd[i+1] && (cmd[i+1]->has_infile || cmd[i+1]->has_heredoc))
		{
			close(pipe_fd[1]);
			prev_pipe[0] = pipe_fd[0];
			prev_pipe[1] = -1;
		}
		else
		{
			prev_pipe[0] = pipe_fd[0];
			prev_pipe[1] = pipe_fd[1];
		}
	}
	else
	{
		prev_pipe[0] = -1;
		prev_pipe[1] = -1;
	}
}

static void execute_command(t_cmd **cmd, t_cmd_path *path, t_token **tokens)
{
	int		i;
	int		is_child;
	int		pipe_fd[2];
	int		prev_pipe[2];
	int		stdin_backup;
	int		stdout_backup;
	pid_t	pid;

	i = -1;
	prev_pipe[0] = -1;
	prev_pipe[1] = -1;

	while (cmd[++i])
	{
		is_child = cmd[i]->has_pipe || (i > 0 && cmd[i-1]->has_pipe);
		if (!is_child)
		{
			stdin_backup = -1;
			stdout_backup = -1;
			if ((cmd[i]->has_appendfile || cmd[i]->has_infile ||
				 cmd[i]->has_outfile) && is_builtin(cmd[i]))
			{
				stdout_backup = dup(STDOUT_FILENO);
				stdin_backup = dup(STDIN_FILENO);
				setup_io_redirections(cmd[i]);
			}
			if (is_builtin(cmd[i]))
			{
				execute_builtin(cmd[i], path, stdin_backup, stdout_backup, tokens);
				continue;
			}
		}
		if (cmd[i]->has_pipe)
		{
			if (pipe(pipe_fd) == -1)
				exit_error("Pipe failed");
		}
		pid = fork();
		if (pid == 0)
		{
			setup_io_redirections_child(cmd[i], pipe_fd, prev_pipe, i);
			execute_builtin_child(cmd[i], path, tokens);
			// Never reaches here
		}
		handle_pipes(pipe_fd, prev_pipe, cmd, i);
	}
	wait_for_children(path);
}

static void wait_for_children(t_cmd_path *path)
{
	int status;

	while (waitpid(-1, &status, 0) > 0)
	{
		if (WIFEXITED(status))
			path->exit_status = WEXITSTATUS(status);
		else if (WIFSIGNALED(status))
			path->exit_status = 128 + WTERMSIG(status);
	}
}
