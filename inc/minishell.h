/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hassende <hassende@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/25 14:24:48 by mdarawsh          #+#    #+#             */
/*   Updated: 2025/03/03 14:59:09 by hassende         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


# ifndef MINISHELL_H
# define MINISHELL_H

// System Libraries
# include <unistd.h>
# include <sys/types.h>
# include <sys/stat.h>
# include <fcntl.h>
# include <stdio.h>
# include <stdlib.h>
# include <signal.h>
# include <sys/wait.h>
# include <readline/readline.h>
# include <readline/history.h>

// Custom Libraries
# include "libft.h"

// Macros
#define MAX_FILENAME 256	// Standard filename length
#define MAX_CMD_LEN 4096	// if the evaluator can get a command more than this size, i'll jump

// Enums
enum	file_type
{
	INFILE,
	OUTFILE
};

// Structures
typedef struct s_cmd_path
{
	char		**path;
	char		**envp;
}	t_cmd_path;

typedef struct s_command
{
	int						has_pipe;
	int						has_infile;
	int						has_outfile;
	int						has_appendfile;
	int						has_heredoc;
	char					*cmd;
	char					**cmd_split;
	char					*infile;
	char					*outfile;
	char					*cmd_path;
	t_cmd_path				*path;
}	t_cmd;

// Function Prototypes
void	exit_error(char *str);
// void	free_all(t_cmd_path *cmd);
void	free_2d(char **str);
void	struc_init(t_cmd_path *path, char **envp);
char	**find_path(char **envp);
void	setup_signals();
void	exec_cmd(char *line_read, t_cmd_path *path);
void	init_cmds(t_cmd **cmd, char *line);
t_cmd	**t_cmd_malloc(char *line_read);
void	setup_command(t_cmd *cmd, t_cmd_path *path);

#endif
