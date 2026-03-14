#include "../organize.h"

/* execute_command — runs in the already-forked child, no extra fork needed */
void execute_command(char **cmd, t_env **env)
{
    char *path;

    if (!cmd || !cmd[0])
        exit(1);
    if (is_builtin(cmd[0]))
        exit(exec_builtin(cmd, env));
    path = build_path(env, cmd[0]);
    if (!path)
    {
        printf("command not found: %s\n", cmd[0]);
        exit(127);
    }
    execve(path, cmd, env_to_array(*env));
    exit(1);
}
