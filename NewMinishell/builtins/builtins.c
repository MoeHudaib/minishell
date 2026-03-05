#include "builtins.h"

int is_builtin(char *cmd, t_env **head)
{
    size_t  len;

    len = ft_strlen(cmd);
    if (ft_strncmp(cmd, "cd", len) == 0)
        return (0);
    else if (ft_strncmp(cmd, "exit", len) == 0)
        return (0);
    else if (ft_strncmp(cmd, "export", len) == 0)
        return (ft_export(head, cmd));
    else if (ft_strncmp(cmd, "unset", len) == 0)
    {
        ft_unset(head, cmd);
        return (0);
    }
    else if (ft_strncmp(cmd, "pwd", len) == 0)
        return (pwd());
    else if (ft_strncmp(cmd, "env", len) == 0)
        return (print_list(*head));
    return (1);
}
