#include "../organize.h"

/* create_processes — forks one child per command, wires pipes */
int create_processes(int len_of_exe, int fd[len_of_exe - 1][2],
                     char ***cmds, t_env **env)
{
    int     i;
    int     j;
    pid_t   pid;

    i = 0;
    while (i < len_of_exe)
    {
        pid = fork();
        if (pid < 0)
            return (1);
        if (pid == 0)
        {
            if (i == 0)                          // first: only stdout
                dup2(fd[i][1], STDOUT_FILENO);
            else if (i == len_of_exe - 1)        // last: only stdin
                dup2(fd[i - 1][0], STDIN_FILENO);
            else                                 // middle: both
            {
                dup2(fd[i - 1][0], STDIN_FILENO);
                dup2(fd[i][1], STDOUT_FILENO);
            }
            j = 0;                               // close all pipe fds in child
            while (j < len_of_exe - 1)
            {
                close(fd[j][0]);
                close(fd[j][1]);
                j++;
            }
            execute_command(cmds[i], env);       // execve inside, never returns
        }
        i++;
    }
    return (0);
}

/* work — creates pipes, launches processes, waits for all */
int work(int len_of_exe, char ***cmds, t_env **env)
{
    int fd[len_of_exe - 1][2];
    int i;

    i = 0;
    while (i < len_of_exe - 1)
    {
        if (pipe(fd[i]) < 0)
        {
            while (--i >= 0)
            {
                close(fd[i][0]);
                close(fd[i][1]);
            }
            return (1);
        }
        i++;
    }
    create_processes(len_of_exe, fd, cmds, env);
    i = 0;                                       // parent closes all pipes
    while (i < len_of_exe - 1)
    {
        close(fd[i][0]);
        close(fd[i][1]);
        i++;
    }
    i = 0;                                       // parent waits for all children
    while (i < len_of_exe)
    {
        wait(NULL);
        i++;
    }
    return (0);
}