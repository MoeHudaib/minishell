#ifndef EXE_H
# define EXE_H

# include <unistd.h>
# include <stdio.h>
# include <stdlib.h>
# include "../builtins/builtins.h"

typedef struct s_exe
{
    char **cmd;
    t_env   *head;
}   t_exe;

int execute_command(char **cmd, t_env **env);

#endif