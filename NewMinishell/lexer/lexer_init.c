#include "lexer.h"

t_lexer *lexer_init(char *data)
{
    t_lexer *lexer;

    if (!data)
        return (NULL);
    lexer = malloc(sizeof(t_lexer));
    if (!lexer)
        return (NULL);
    lexer->double_quote = 0;
    lexer->single_quote = 0;
    lexer->token = ft_strdup(data);
    if (!lexer->token)
    {
        free(lexer);
        return (NULL);
    }
    lexer->next = NULL;
    return (lexer);
}

t_lexer	*add_new_token_front(t_lexer **lexer_head, char *data)
{
    t_lexer	*new_lexer;

    if (!lexer_head || !*lexer_head || !data)
    {
        return (NULL);
    }
    new_lexer = lexer_init(data);
    if (!new_lexer)
    {
        return (NULL);
    }
    new_lexer->next = *lexer_head;
    *lexer_head = new_lexer;
    return (*lexer_head);
}

t_lexer	*add_new_token_back(t_lexer **lexer_head, char *data)
{
    t_lexer	*new_lexer;

    if (!lexer_head || !*lexer_head || !data)
    {
        return (NULL);
    }
    new_lexer = lexer_init(data);
    if (!new_lexer)
    {
        return (NULL);
    }
    return (add_token_back(lexer_head, new_lexer));
}

t_lexer *add_token_back(t_lexer **lexer_head,t_lexer *lexer)
{
    t_lexer *current;

    if (!lexer_head || !*lexer_head || !lexer)
        return (NULL);
    current = *lexer_head;
    while (current->next)
        current = current->next;
    current->next = lexer;
    return (*lexer_head);
}

void	delete_lexer(t_lexer **head)
{
	t_lexer	*current;
	t_lexer	*tmp;

	if (!head || !*head)
	{
		return ;
	}
	current = *head;
	while (current)
	{
		tmp = current;
		current = current->next;
        if (tmp->token)
            free(tmp->token);
        free(tmp);
		tmp = NULL;
	}
}