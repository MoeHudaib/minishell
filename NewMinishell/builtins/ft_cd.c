#include "../organize.h"

int ft_cd(char **cmd, t_env **env)
{
    char *path;

    if (!cmd[1])
        path = get_env_value("HOME", env_to_array(*env));
    else
        path = cmd[1];

    if (chdir(path) != 0)
    {
        perror("cd");
        return (1);
    }

    return (0);
}
