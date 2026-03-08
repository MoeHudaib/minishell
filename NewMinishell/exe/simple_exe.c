#include "exe.h"

int execute_command(char **cmd, t_env **env)
{
    pid_t pid;
    char *path;

    if (!cmd || !cmd[0])
        return 0;

    if (is_builtin(cmd[0]))
        return exec_builtin(cmd, env);

    pid = fork();

    if (pid == 0)
    {
        path = build_path(env, cmd[0]);

        if (!path)
        {
            printf("command not found\n");
            exit(127);
        }

        execve(path, cmd, env_to_array(*env));
        exit(1);
    }

    waitpid(pid, NULL, 0);

    return 0;
}