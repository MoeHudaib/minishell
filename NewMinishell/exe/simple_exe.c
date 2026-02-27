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
static void first_child_go(char *path, char **cmd, char **env)
{
    if (access(path, F_OK | X_OK) != 0)
    {
        printf("command Not Found !\n");
        cleanup(cmd, path);
        exit(1);
    }
    execve(path, cmd, env);
}

static void  presetup(char *av1, char **env)
{
    char    **cmd;
    char    *path;

    int pid = fork();
    if (pid == 0)
    {
        cmd = ft_split(av1, ' ');
        path = build_path(env, cmd[0]);
        first_child_go(path, cmd, env);
    }
    waitpid(pid, NULL, 0);
    printf("Child process has ended successfully!\n");
}

int simple_exe(int ac, char **av, char **env)
{
    int     *fd;

    presetup(av[1], env);
    return (0);
}