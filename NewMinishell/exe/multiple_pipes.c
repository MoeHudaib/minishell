#include "exe.h"

int create_processes(int len_of_exe, int fd[len_of_exe - 1][2])
{
    int i;
    pid_t pid;

    i = 0;
    while (i < len_of_exe)
    {
        pid = fork();

        if (pid == 0)
        {
            // first command
            if (i == 0)
            {
                dup2(fd[i][1], STDOUT_FILENO);
            }

            // last command
            else if (i == len_of_exe - 1)
            {
                dup2(fd[i - 1][0], STDIN_FILENO);
            }

            // middle commands
            else
            {
                dup2(fd[i - 1][0], STDIN_FILENO);
                dup2(fd[i][1], STDOUT_FILENO);
            }

            int j = 0;
            while (j < len_of_exe - 1)
            {
                close(fd[j][0]);
                close(fd[j][1]);
                j++;
            }

            execute_command(i); // your command executor
            exit(0);
        }

        i++;
    }

    i = 0;
    while (i < len_of_exe)
    {
        wait(NULL);
        i++;
    }

    return (0);
}

int work(int len_of_exe, )
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

    create_processes(len_of_exe, fd);

    i = 0;
    while (i < len_of_exe - 1)
    {
        close(fd[i][0]);
        close(fd[i][1]);
        i++;
    }

    return (0);
}
