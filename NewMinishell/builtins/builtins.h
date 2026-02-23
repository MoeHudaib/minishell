#ifndef BUILTINS_H
# define BUILTINS_H

# include <fcntl.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <unistd.h>
# include <ctype.h>
# include "../parser/parse.h"
# include "../lexer/lexer.h"

int pwd(void);
int is_builtin(const char *cmd);

#endif