#ifndef BUILTINS_H
# define BUILTINS_H

# include <fcntl.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <unistd.h>
# include <ctype.h>
# include "../expander/expander.h"


int ft_pwd(void);
int ft_exit(char **cmd);
int ft_env(t_env *env);
int ft_echo(char **cmd);
int ft_cd(char **cmd, t_env **env);
int is_builtin(char *cmd);
int exec_builtin(char **cmd, t_env **env);

#endif