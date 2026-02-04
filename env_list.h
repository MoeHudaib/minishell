#ifndef ENV_LIST_H
# define ENV_LIST_H

# include <unistd.h>
# include <stdlib.h>
# include <stdio.h>

typedef struct  s_env_list
{
    char    *path;
    struct s_env_list   *next;
}   t_env;



#endif