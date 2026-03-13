#include "../organize.h"

int is_valid_var_char(char c)
{
    return (ft_isalnum(c) || c == '_');
}

char *get_env_value(char *var, char **env)
{
    int i;
    int len;

    i = 0;
    len = ft_strlen(var);
    while (env[i])
    {
        if (ft_strncmp(env[i], var, len) == 0
            && env[i][len] == '=')
            return (ft_strdup(env[i] + len + 1));
        i++;
    }
    return (ft_strdup(""));
}

char *get_exit_status(int status)
{
    return (ft_itoa(status)); // will then be replaced with real exit status when we execute the commands
}