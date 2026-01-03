#include "lexer.h"

void	*free_tokens(char **tokens, char *line, t_lexer **lexer_head)
{
	int	i;

	if (tokens)
	{
		i = 0;
		while (tokens[i])
		{
			free(tokens[i]);
			i++;
		}
		free(tokens);
	}
	if (line)
		free(line);
	if (lexer_head)
		delete_lexer(lexer_head);
	return (NULL);
}
