#include "minishell.h"

static int	exit_isdigit(char *str)
{
	int	i;

	i = 0;
	if (str[i] == '-')
		i++;
	while (ft_isdigit(str[i]))
		i++;
	if (i != ft_strlen(str))
		return (0);
	return (1);
}

int	do_exit(t_cmd *cmd)
{
	int	exit_code;

	if (cmd->cmd_split[1] == NULL)
		exit(0);
	if (cmd->cmd_split[2] != NULL)
	{
		ft_putstr_fd("minishell: exit: too many arguments\n", 2);
		return (0);
	}
	if (!exit_isdigit(cmd->cmd_split[1]))
	{
		ft_putstr_fd("minishell: exit: ", 2);
		ft_putstr_fd(cmd->cmd_split[1], 2);
		ft_putstr_fd(": numeric argument required\n", 2);
		exit (255);
	}
	exit_code = ft_atoi(cmd->cmd_split[1]);
	exit_code %= 256;
	exit (exit_code);
}
