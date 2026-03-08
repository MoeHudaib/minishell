#ifndef EXPANDER_H
# define EXPANDER_H


# include <stdlib.h>
# include "../parser/parse.h"

char *expand_dollar(const char *str, int *i, char quote, int last_status, char **env);
int     is_valid_var_char(char c);
char    *get_env_value(char *var, char **env);
char    *get_exit_status(int status);
char *expand_token(char *token, int last_status, char **env);
void expand_lexer_tokens(t_lexer *tokens, int last_status, t_env *env);

#endif