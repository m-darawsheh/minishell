/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hassende <hassende@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/25 14:24:48 by mdarawsh          #+#    #+#             */
/*   Updated: 2025/02/25 18:22:56 by hassende         ###   ########.fr       */
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

typedef struct s_cmd
{
	char	*cmd;
	char	**cmd_split;
	char	**path;
	char	**envp;
	pid_t	pid;
}	t_cmd;

void	exit_error(char *str);
void	free_all(t_cmd *cmd);
void	free_2d(char **str);
void	struc_init(t_cmd *cmd);
#endif
