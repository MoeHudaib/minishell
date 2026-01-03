#ifndef LEXER_H
# define LEXER_H

# include <stdio.h>
# include <unistd.h>
# include <stdlib.h>
# include <sys/wait.h>
# include <signal.h>
# include <readline/readline.h>
# include <readline/history.h>
# include "../libs/libft/libft.h"

typedef struct s_lexer
{
    char			*token;
    int				single_quote;
    int				double_quote;
    struct s_lexer	*next;
}   t_lexer;

char		**split_with_quotes(const char *str);
t_lexer		*lexer_init(char *data);
t_lexer		*add_new_token_front(t_lexer **lexer_head, char *data);
t_lexer		*add_new_token_back(t_lexer **lexer_head, char *data);
t_lexer		*add_token_back(t_lexer **lexer_head,t_lexer *lexer);
void		delete_lexer(t_lexer **head);
void		*free_tokens(char **tokens, char *line, t_lexer **lexer_head);
char		*read_full_input(char *str);
t_lexer		*lex_line(char *line);
void		print_tokens(t_lexer *lexer_head);

#endif