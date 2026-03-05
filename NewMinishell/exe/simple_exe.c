#include "exe.h"
static void cleanup(char **cmd, char *path)
{
    int i;

    i = 0;
    while (cmd[i])
    {
        free(cmd[i]);
        i++;
    }
    free(cmd);
    free(path);
}
static void first_child_go(char *path, char **cmd, t_env **head)
{
    char **env;

    if (access(path, F_OK | X_OK) != 0)
    {
        printf("command Not Found !\n");
        cleanup(cmd, path);
        exit(1);
    }
    env = env_to_array(*head);
    execve(path, cmd, env);
}

static void  presetup(t_env **head, char **cmd)
{
    char    *path;

    int pid = fork();
    if (pid == 0)
    {
        path = build_path(head, cmd[0]);
        first_child_go(path, cmd, head);
    }
    waitpid(pid, NULL, 0);
    printf("Child process has ended successfully!\n");
}

int simple_exe(t_env **head, char **av)
{
    int     *fd;

    presetup(head, av);
    return (0);
}