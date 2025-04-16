/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdarawsh <mdarawsh@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/25 14:24:48 by mdarawsh          #+#    #+#             */
/*   Updated: 2025/04/16 19:47:09 by mdarawsh         ###   ########.fr       */
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
# include <sys/ioctl.h>
# include <readline/readline.h>
# include <readline/history.h>

// Custom Libraries
# include "libft.h"

// Macros
#define MAX_FILENAME 4096	// 256 is the Standard filename length, but the evalutaor could give a full path
#define MAX_CMD_LEN 4096	// if the evaluator can get a command more than this size, i'll jump
#define MAX_ENV_NAME 512	// 256 is the length limit for an ENV variable name, but just to be safe :D.

// Enums
enum	file_type
{
	INFILE,
	OUTFILE
};

typedef enum e_token_type {
	TOKEN_WORD,	// Regular word or quoted string
	TOKEN_PIPE,
	TOKEN_REDIR_IN,
	TOKEN_REDIR_OUT,
	TOKEN_APPEND,
	TOKEN_HEREDOC
} t_token_type;

// Structures
typedef struct s_token {
	char *value;
	int was_in_double_quotes;
	int the_index_must_be_split_on_space;
	t_token_type type;
} t_token;

typedef struct s_lexer {
	t_token **tokens;
	char	buffer[MAX_CMD_LEN];
	char	*line;
	int		count;
	int		buffer_pos;
	int		in_quotes;
	int		i;
} t_lexer;


typedef struct s_cmd_path
{
	char		**path;
	char		**envp;
	int			exit_status;
}	t_cmd_path;

typedef struct s_command
{
	int						heredoc_fd;
	int						has_pipe;
	int						has_infile;
	int						has_outfile;
	int						has_appendfile;
	int						has_heredoc;
	int						builtin;
	int						skip_exec;
	char					*cmd;
	char					**cmd_split;
	char					*infile;
	char					*outfile;
	char					*cmd_path;
	char					*delimiter;
	t_cmd_path				*path;
}	t_cmd;

// Function Prototypes
t_cmd	**t_cmd_malloc(char *line_read, t_cmd_path *path);
char	**realloc_2d(char **str , int old_size , int new_size);
char	**find_path(char **envp);
int		do_exit(t_cmd *cmd);
int		do_cd(t_cmd *cmd, t_cmd_path *path);
int		is_builtin(t_cmd *cmd);
void	*print_and_null(char *str);
void	exit_error(char *str);
void	free_2d(char **str);
void	struc_init(t_cmd_path *path, char **envp);
void	setup_signals();
void	exec_cmd(char *line_read, t_cmd_path *path);
void	init_cmds(t_cmd **cmd, char *line);
void	setup_command(t_cmd *cmd, t_cmd_path *path);
void	pwd_handle(t_cmd_path *path);
void	export_handle(t_cmd *cmd ,t_cmd_path *path, t_token **tokens);
void	do_echo(t_cmd *cmd, t_token **tokens);
void	free_cmds(t_cmd **cmd);
void	handle_heredoc(t_cmd *cmd);
void	expander(t_token ***tokens, t_cmd_path *path);
void	print_env(t_cmd_path *path);
void	handle_unset(t_cmd_path *path, t_token **tokens);
void	split_for_expand2(t_cmd **cmd, t_token **tokens);


// Lexer
t_token	**tokenize(char *line_read, int flag);
int		parse_token(t_token ***tokens, t_cmd **cmd);
int		process_quotes(t_lexer *lexer);
void	init_lexer(t_lexer *lexer, char *line);
void	add_token(t_lexer *lexer, char *value, t_token_type type);
void	add_word_token(t_lexer *lexer);
void	add_special_double_token(t_lexer *lexer);
void	free_tokens(t_token **tokens);
#endif
