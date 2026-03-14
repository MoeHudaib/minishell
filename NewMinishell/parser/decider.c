#include "../organize.h"

extern volatile sig_atomic_t g_sig;

int is_parent_builtin(char *cmd)
{
    if (!cmd)
        return (0);
    return (!ft_strcmp(cmd, "cd")
        || !ft_strcmp(cmd, "export")
        || !ft_strcmp(cmd, "unset")
        || !ft_strcmp(cmd, "exit"));
}

int apply_redirections(t_cmd *cmd)
{
    t_redir *redir;
    int     fd;

    redir = cmd->redirs;
    while (redir)
    {
        if (redir->type == TOKEN_REDIR_OUT)
        {
            fd = open(redir->file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
            if (fd < 0)
            {
                ft_putstr_fd("minishell: ", STDERR_FILENO);
                ft_putstr_fd(redir->file, STDERR_FILENO);
                ft_putendl_fd(": No such file or directory", STDERR_FILENO);
                return (1);
            }
            dup2(fd, STDOUT_FILENO);
            close(fd);
        }
        else if (redir->type == TOKEN_REDIR_APPEND)
        {
            fd = open(redir->file, O_WRONLY | O_CREAT | O_APPEND, 0644);
            if (fd < 0)
            {
                ft_putstr_fd("minishell: ", STDERR_FILENO);
                ft_putstr_fd(redir->file, STDERR_FILENO);
                ft_putendl_fd(": No such file or directory", STDERR_FILENO);
                return (1);
            }
            dup2(fd, STDOUT_FILENO);
            close(fd);
        }
        else if (redir->type == TOKEN_REDIR_IN)
        {
            fd = open(redir->file, O_RDONLY);
            if (fd < 0)
            {
                ft_putstr_fd("minishell: ", STDERR_FILENO);
                ft_putstr_fd(redir->file, STDERR_FILENO);
                ft_putendl_fd(": No such file or directory", STDERR_FILENO);
                return (1);
            }
            dup2(fd, STDIN_FILENO);
            close(fd);
        }
        else if (redir->type == TOKEN_HEREDOC)
        {
            if (redir->fd < 0)      // -1 means either not prepared or already merged
            {
                redir = redir->next;
                continue ;
            }
            dup2(redir->fd, STDIN_FILENO);
            close(redir->fd);
            redir->fd = -1;
        }
        redir = redir->next;
    }
    return (0);
}

static int count_commands(t_cmd *cmds)
{
    int count;

    count = 0;
    while (cmds)
    {
        count++;
        cmds = cmds->next;
    }
    return (count);
}

int are_we_gonna_split(t_lexer *tokens, t_env **env)
{
    t_cmd   *cmd_list;
    int     len;
    pid_t   pid;
    int     status;
    int     last_status;

    if (!tokens)
        return (1);
    cmd_list = parse(tokens);
    if (!cmd_list)
        return (1);
    len = count_commands(cmd_list);
    if (len == 1)
    {
        // guard: args may be NULL when the input is a bare redirection
        if (cmd_list->args && cmd_list->args[0]
            && is_parent_builtin(cmd_list->args[0]))
        {
            last_status = exec_builtin(cmd_list->args, env);
            free_cmd_list(cmd_list);
            return (last_status);
        }
        if (!prepare_heredocs(cmd_list))
        {
            free_cmd_list(cmd_list);
            g_sig = 0;
            signals_interactive();
            return (130);
        }
        signals_child();
        pid = fork();
        if (pid < 0)
        {
            signals_interactive();
            free_cmd_list(cmd_list);
            return (1);
        }
        if (pid == 0)
        {
            signals_reset_child();
            if (apply_redirections(cmd_list))
                exit(1);
            // bare redirection with no command: bash creates the file and exits 0
            if (!cmd_list->args || !cmd_list->args[0])
                exit(0);
            execute_command(cmd_list->args, env);
        }
        waitpid(pid, &status, 0);
        free_cmd_list(cmd_list);
        if (WIFSIGNALED(status))
        {
            last_status = 128 + WTERMSIG(status);
            if (WTERMSIG(status) == SIGQUIT)
                write(STDERR_FILENO, "Quit: 3\n", 8);
        }
        else
            last_status = WEXITSTATUS(status);
        signals_interactive();
        return (last_status);
    }
    return (work(len, cmd_list, env));
}