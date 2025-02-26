/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hassende <hassende@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/25 14:24:48 by mdarawsh          #+#    #+#             */
/*   Updated: 2025/02/26 15:20:40 by hassende         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


# ifndef MINISHELL_H
# define MINISHELL_H

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

# include "libft.h"

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
	t_cmd_path				*cmd_path;
	struct s_command		*next;
}	t_cmd;


void	exit_error(char *str);
void	free_all(t_cmd_path *cmd);
void	free_2d(char **str);
void	struc_init(t_cmd_path *cmd, char **envp);
char	**find_path(char **envp);
void	setup_signals();

#endif
