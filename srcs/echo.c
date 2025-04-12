#include "minishell.h"

static int	check_all_n(char *str)
{
	int i;

	i = 1;
	while (str[i] == 'n')
		i++;
	if (str[i] == '\0')
		return (1);
	return (0);
}

void	do_echo(t_cmd *cmd, t_token **tokens)
{
	(void)cmd;
	int i;
	int	n_flag;

	i = 1;
	n_flag = 0;

	while (tokens[i]->value && tokens[i]->value[0] == '-' &&
		tokens[i]->value[1] == 'n' && check_all_n(tokens[i]->value))
 	{
		n_flag = 1;
		i++;
 	}
	while (tokens[i] && tokens[i]->value)
	{
		ft_putstr_fd(tokens[i]->value, STDOUT_FILENO);
		if ( tokens[i + 1] && tokens[i + 1]->value) /* If not last argument */
			ft_putchar_fd(' ', STDOUT_FILENO);
		i++;
	}
	if (!n_flag)
		ft_putchar_fd('\n', STDOUT_FILENO);
}
