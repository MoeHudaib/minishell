#include "builtins.h"

int ft_exit(char **cmd)
{
    int status = 0;

    if (cmd[1])
        status = ft_atoi(cmd[1]);

    printf("exit\n");
    exit(status);
}