#ifndef EXE_H
# define EXE_H

# include <unistd.h>
# include <stdio.h>
# include <stdlib.h>
# include "../builtins/builtins.h"

typedef struct s_fd
{
    int fd_in;
    int fd_out;
    t_env   *head;
}   t_fd;

int execute_command(char **cmd, t_env **env);

#endif