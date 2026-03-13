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

void execute_command(char **cmd, t_env **env);
int create_processes(int len_of_exe, int fd[len_of_exe - 1][2],
                     char ***cmds, t_env **env);
int work(int len_of_exe, char ***cmds, t_env **env);

#endif