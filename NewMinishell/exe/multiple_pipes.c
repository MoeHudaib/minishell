#include "../organize.h"

/* create_processes — forks one child per command, wires pipes */
int create_processes(int len_of_exe, int fd[len_of_exe - 1][2],
        t_cmd *cmd_list, t_env **env)
{
    int     i;
    int     j;
    pid_t   pid;
    t_cmd   *current;

    i = 0;
    current = cmd_list;
    while (i < len_of_exe)
    {
        pid = fork();
        if (pid < 0)
            return (1);
        if (pid == 0)
        {
            if (i == 0)
                dup2(fd[i][1], STDOUT_FILENO);
            else if (i == len_of_exe - 1)
                dup2(fd[i - 1][0], STDIN_FILENO);
            else
            {
                dup2(fd[i - 1][0], STDIN_FILENO);
                dup2(fd[i][1], STDOUT_FILENO);
            }
            j = 0;
            while (j < len_of_exe - 1)
            {
                close(fd[j][0]);
                close(fd[j][1]);
                j++;
            }
            apply_redirections(current);        // apply per-command redirections
            execute_command(current->args, env);
        }
        current = current->next;
        i++;
    }
    return (0);
}

/* work — creates pipes, launches processes, waits for all */
int work(int len_of_exe, t_cmd *cmd_list, t_env **env)
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
    create_processes(len_of_exe, fd, cmd_list, env);
    i = 0;
    while (i < len_of_exe - 1)
    {
        close(fd[i][0]);
        close(fd[i][1]);
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