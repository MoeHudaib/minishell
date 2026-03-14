#include "../organize.h"

int create_processes(int len_of_exe, int fd[len_of_exe - 1][2],
        t_cmd *cmd_list, t_env **env, pid_t *pids)
{
    int     i;
    int     j;
    t_cmd   *current;

    i = 0;
    current = cmd_list;
    while (i < len_of_exe)
    {
        pids[i] = fork();
        if (pids[i] < 0)
            return (1);
        if (pids[i] == 0)
        {
            signals_reset_child();
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
            if (apply_redirections(current))
                exit(1);
            execute_command(current->args, env);
        }
        current = current->next;
        i++;
    }
    return (0);
}

int work(int len_of_exe, t_cmd *cmd_list, t_env **env)
{
    int     fd[len_of_exe - 1][2];
    pid_t   pids[len_of_exe];
    int     i;
    int     status;
    int     last_status;

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
            free_cmd_list(cmd_list);
            return (1);
        }
        i++;
    }
    if (!prepare_heredocs(cmd_list))    // before signals_child + fork
    {
        i = 0;
        while (i < len_of_exe - 1)
        {
            close(fd[i][0]);
            close(fd[i][1]);
            i++;
        }
        free_cmd_list(cmd_list);
        g_sig = 0;
        signals_interactive();
        return (130);
    }
    signals_child();
    create_processes(len_of_exe, fd, cmd_list, env, pids);
    i = 0;
    while (i < len_of_exe - 1)
    {
        close(fd[i][0]);
        close(fd[i][1]);
        i++;
    }
    last_status = 0;
    i = 0;
    while (i < len_of_exe)
    {
        waitpid(pids[i], &status, 0);
        if (i == len_of_exe - 1)
        {
            if (WIFSIGNALED(status))
            {
                last_status = 128 + WTERMSIG(status);
                if (WTERMSIG(status) == SIGQUIT)
                    write(STDERR_FILENO, "Quit: 3\n", 8);
            }
            else
                last_status = WEXITSTATUS(status);
        }
        i++;
    }
    signals_interactive();
    free_cmd_list(cmd_list);
    return (last_status);
}