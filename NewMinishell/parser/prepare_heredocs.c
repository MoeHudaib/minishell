#include "../organize.h"

extern volatile sig_atomic_t g_sig;

static int  read_heredoc(t_redir *redir, int write_fd)
{
    char    *line;
    char    *delimiter;

    delimiter = redir->file;
    while (1)
    {
        signals_heredoc();
        line = readline("> ");
        if (!line)
            break ;
        if (g_sig == SIGINT)
        {
            free(line);
            return (0);
        }
        if (ft_strcmp(line, delimiter) == 0)
        {
            free(line);
            break ;
        }
        write(write_fd, line, ft_strlen(line));
        write(write_fd, "\n", 1);
        free(line);
    }
    return (1);
}

static int prepare_cmd_heredocs(t_cmd *cmd)
{
    t_redir *redir;
    int     pipe_fd[2];
    int     first_read_fd;
    int     combined_pipe[2];
    char    buf[4096];
    ssize_t n;

    // count heredocs
    int heredoc_count = 0;
    redir = cmd->redirs;
    while (redir)
    {
        if (redir->type == TOKEN_HEREDOC)
            heredoc_count++;
        redir = redir->next;
    }

    // if only one heredoc, original logic is fine
    if (heredoc_count <= 1)
    {
        redir = cmd->redirs;
        while (redir)
        {
            if (redir->type == TOKEN_HEREDOC)
            {
                if (pipe(pipe_fd) < 0)
                    return (0);
                if (!read_heredoc(redir, pipe_fd[1]))
                {
                    close(pipe_fd[0]);
                    close(pipe_fd[1]);
                    return (0);
                }
                close(pipe_fd[1]);
                redir->fd = pipe_fd[0];
            }
            redir = redir->next;
        }
        return (1);
    }

    // multiple heredocs: create one combined pipe
    if (pipe(combined_pipe) < 0)
        return (0);
    redir = cmd->redirs;
    while (redir)
    {
        if (redir->type == TOKEN_HEREDOC)
        {
            if (pipe(pipe_fd) < 0)
            {
                close(combined_pipe[0]);
                close(combined_pipe[1]);
                return (0);
            }
            if (!read_heredoc(redir, pipe_fd[1]))
            {
                close(pipe_fd[0]);
                close(pipe_fd[1]);
                close(combined_pipe[0]);
                close(combined_pipe[1]);
                return (0);
            }
            close(pipe_fd[1]);
            // copy this heredoc's content into the combined pipe
            while ((n = read(pipe_fd[0], buf, sizeof(buf))) > 0)
                write(combined_pipe[1], buf, n);
            close(pipe_fd[0]);
            redir->fd = -1;             // mark as handled
        }
        redir = redir->next;
    }
    close(combined_pipe[1]);

    // give the combined read-end to the first heredoc redir
    redir = cmd->redirs;
    while (redir)
    {
        if (redir->type == TOKEN_HEREDOC)
        {
            redir->fd = combined_pipe[0];
            break;
        }
        redir = redir->next;
    }
    return (1);
}

int prepare_heredocs(t_cmd *cmd_list)
{
    t_cmd   *current;

    current = cmd_list;
    while (current)
    {
        if (!prepare_cmd_heredocs(current))
            return (0);
        current = current->next;
    }
    return (1);
}