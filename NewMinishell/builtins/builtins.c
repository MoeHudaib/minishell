#include "builtins.h"

int is_builtin(const char *cmd)
{
    size_t  len;

    len = ft_strlen(cmd);
    if (ft_strncmp(cmd, "cd", len) == 0)
        return (0);
    else if (ft_strncmp(cmd, "exit", len) == 0)
        return (0);
    else if (ft_strncmp(cmd, "export", len) == 0)
        return (0);
    else if (ft_strncmp(cmd, "unset", len) == 0)
        return (0);        
    else if (ft_strncmp(cmd, "pwd", len) == 0)
        return (pwd());
    return (1);
}
