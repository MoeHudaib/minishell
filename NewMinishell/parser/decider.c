#include "../organize.h"

/* apply redirections for a command before execve */
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
                return (1);
            dup2(fd, STDOUT_FILENO);
            close(fd);
        }
        else if (redir->type == TOKEN_REDIR_APPEND)
        {
            fd = open(redir->file, O_WRONLY | O_CREAT | O_APPEND, 0644);
            if (fd < 0)
                return (1);
            dup2(fd, STDOUT_FILENO);
            close(fd);
        }
        else if (redir->type == TOKEN_REDIR_IN)
        {
            fd = open(redir->file, O_RDONLY);
            if (fd < 0)
                return (1);
            dup2(fd, STDIN_FILENO);
            close(fd);
        }
        else if (redir->type == TOKEN_HEREDOC)
        {
            // handled separately
        }
        redir = redir->next;
    }
    return (0);
}

/* counts how many commands in the list */
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

/* extracts char*** from t_cmd list */
static char ***cmds_to_array(t_cmd *list, int len)
{
    char    ***cmds;
    int     i;

    cmds = malloc(sizeof(char **) * (len + 1));
    if (!cmds)
        return (NULL);
    i = 0;
    while (list && i < len)
    {
        cmds[i] = list->args;
        list = list->next;
        i++;
    }
    cmds[i] = NULL;
    return (cmds);
}

int are_we_gonna_split(t_lexer *tokens, t_env **env)
{
    t_cmd   *cmd_list;
    int     len;
    pid_t   pid;
    int     status;

    if (!tokens)
        return (1);
    cmd_list = parse(tokens);
    if (!cmd_list)
        return (1);
    len = count_commands(cmd_list);
    if (len == 1)
    {
        pid = fork();
        if (pid < 0)
            return (1);
        if (pid == 0)
        {
            apply_redirections(cmd_list);
            execute_command(cmd_list->args, env);
        }
        waitpid(pid, &status, 0);
        free_cmd_list(cmd_list);
        return (WEXITSTATUS(status));
    }
    return (work(len, cmd_list, env));
}