#include "lexer.h"

int	has_unclosed_quotes(const char *s)
{
	int		i = 0;
	char	quote = 0;

	while (s[i])
	{
		if (s[i] == '\\' && quote != '\'')
		{
			if (s[i + 1])
				i++;
		}
		else if (s[i] == '\'' || s[i] == '"')
		{
			if (!quote)
				quote = s[i];
			else if (quote == s[i])
				quote = 0;
		}
		i++;
	}
	return (quote != 0);
}

char	*read_full_input(char *str)
{
	char *line;
	char *temp;
	char *cwd = ft_strjoin(str, "> ");
	char *full = readline(cwd);
	while (full && has_unclosed_quotes(full))
	{
		temp = readline("> ");
		if (!temp)
			break;
		char *joined = malloc(strlen(full) + strlen(temp) + 2);
		sprintf(joined, "%s\n%s", full, temp);
		free(full);
		free(temp);
		full = joined;
	}
	free(cwd);
	return (full);
}

static t_lexer	*create_lexer_list(char **tokens)
{
	t_lexer	*head;
	t_lexer	*current;
	int		i;

	i = 0;
	head = lexer_init(tokens[i++]);
	if (!head)
		return (NULL);
	current = head;
	while (tokens[i])
	{
		current = add_new_token_back(&head, tokens[i++]);
		if (!current)
			return (free_tokens(tokens, NULL, &head));
	}
	return (head);
}

t_lexer	*lex_line(char *line)
{
	t_lexer	*lexer_head;
	char	**tokens;

	if (!line)
		return (NULL);
	tokens = split_with_quotes(line);
	if (!tokens)
		return (NULL);
	lexer_head = create_lexer_list(tokens);
	free_tokens(tokens, NULL, NULL);
	return (lexer_head);
}

void	print_tokens(t_lexer *lexer_head)
{
	t_lexer	*current;

	if (!lexer_head)
	{
		return ;
	}
	current = lexer_head;
	while (current)
	{
		printf("%s", current->token);
		if (current->next)
			printf(" -> ");
		current = current->next;
	}
	printf("\n");
}