#include "minishell.h"

// ? < infile echo | cat -e > outfile

char	**tokenize(char *line_read)
{
	t_token	**tokens;
	int		token_count;
	int		i;
	int		in_quote;

	tokens = malloc(sizeof(t_token *) * ft_strlen(line_read));
	if (!tokens)
		exit_error("Malloc failed");
	token_count = 0;
	i = 0;
	in_quote = 0;
	while (line_read[i])
	{
		if (line_read[i] == '\'' || line_read[i] == '\"')
		{
			in_quote = !in_quote;
			i++;
			continue ;
		}

	}
}
