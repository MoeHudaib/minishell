#include "quotes.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <fcntl.h>

size_t get_size(char *str)
{
    size_t count = 0;
    int i = 0;

    while (str[i])
    {
        if (str[i] == '>' || str[i] == '<')
        {
            if ((str[i] == '>' && str[i + 1] == '>') ||
                (str[i] == '<' && str[i + 1] == '<'))
                i++; // skip extra char for double operator
            count++;
        }
        else if (!isspace(str[i]))
        {
            count++; // a normal token
            // skip to next space or redirection
            while (str[i] && !isspace(str[i]) && str[i] != '>' && str[i] != '<')
                i++;
            i--; // adjust for outer loop increment
        }
        i++;
    }
    return count;
}

char **tokenize(char *str)
{
    int i = 0;
    int count = 0;
    size_t toksize = get_size(str);
    char **tokens = malloc(sizeof(char*) * (toksize + 1));
    if (!tokens)
        return NULL;

    while (str[i])
    {
        // skip spaces
        while (isspace(str[i]))
            i++;
        if (!str[i])
            break;

        // handle redirections
        if (str[i] == '>' || str[i] == '<')
        {
            if (str[i] == '>' && str[i + 1] == '>')
                tokens[count++] = strdup(">>"), i += 2;
            else if (str[i] == '<' && str[i + 1] == '<')
                tokens[count++] = strdup("<<"), i += 2;
            else
            {
                char tmp[2] = {str[i], '\0'};
                tokens[count++] = strdup(tmp);
                i++;
            }

            // save the following file/delimiter token
            while (isspace(str[i])) i++; // skip spaces
            if (str[i])
            {
                int start = i;
                while (str[i] && !isspace(str[i]) && str[i] != '>' && str[i] != '<')
                    i++;
                int len = i - start;
                char *file = malloc(len + 1);
                strncpy(file, str + start, len);
                file[len] = '\0';
                tokens[count++] = file;
            }
        }
        else // normal token
        {
            int start = i;
            while (str[i] && !isspace(str[i]) && str[i] != '>' && str[i] != '<')
                i++;
            int len = i - start;
            char *token = malloc(len + 1);
            strncpy(token, str + start, len);
            token[len] = '\0';
            tokens[count++] = token;
        }
    }

    tokens[count] = NULL;
    return tokens;
}

t_command *parse_commands(char **tokens)
{
    t_command *head = NULL, *curr = NULL;
    int i = 0;

    while (tokens[i])
    {
        t_command *cmd = calloc(1, sizeof(t_command));
        cmd->args = malloc(sizeof(char*) * 256);
        int argc = 0;

        while (tokens[i] && strcmp(tokens[i], "|") != 0 &&
               strcmp(tokens[i], "&&") != 0 &&
               strcmp(tokens[i], "||") != 0)
        {
            if (strcmp(tokens[i], "<") == 0 && tokens[i+1])
                cmd->infile = strdup(tokens[++i]);
            else if (strcmp(tokens[i], ">") == 0 && tokens[i+1])
                cmd->outfile = strdup(tokens[++i]), cmd->append = 0;
            else if (strcmp(tokens[i], ">>") == 0 && tokens[i+1])
                cmd->outfile = strdup(tokens[++i]), cmd->append = 1;
            else if (strcmp(tokens[i], "<<") == 0 && tokens[i+1])
                cmd->heredoc = strdup(tokens[++i]);
            else if (strcmp(tokens[i], "2>") == 0 && tokens[i+1])
                cmd->errfile = strdup(tokens[++i]), cmd->err_append = 0;
            else if (strcmp(tokens[i], "2>>") == 0 && tokens[i+1])
                cmd->errfile = strdup(tokens[++i]), cmd->err_append = 1;
            else
                cmd->args[argc++] = strdup(tokens[i]);
            i++;
        }

        cmd->args[argc] = NULL;

        // link commands
        if (!head) head = cmd;
        if (curr) curr->next = cmd;
        curr = cmd;

        if (tokens[i] && strcmp(tokens[i], "|") == 0)
            i++;
        else if (tokens[i] && (strcmp(tokens[i], "&&") == 0 || strcmp(tokens[i], "||") == 0))
            i++;
    }

    return head;
}

// Helper: apply redirections / heredocs when no command exists
int apply_redirections_only(t_command *cmd)
{
    int fd;

    // stdout
    if (cmd->outfile)
    {
        fd = cmd->append ? 
             open(cmd->outfile, O_CREAT | O_WRONLY | O_APPEND, 0644) :
             open(cmd->outfile, O_CREAT | O_WRONLY | O_TRUNC, 0644);
        if (fd < 0) { perror(cmd->outfile); return 1; }
        close(fd);
    }

    // stderr
    if (cmd->errfile)
    {
        fd = cmd->err_append ?
             open(cmd->errfile, O_CREAT | O_WRONLY | O_APPEND, 0644) :
             open(cmd->errfile, O_CREAT | O_WRONLY | O_TRUNC, 0644);
        if (fd < 0) { perror(cmd->errfile); return 1; }
        close(fd);
    }

    // stdin
    if (cmd->infile)
    {
        fd = open(cmd->infile, O_RDONLY);
        if (fd < 0) { perror(cmd->infile); return 1; }
        close(fd);
    }

    // heredoc
    if (cmd->heredoc)
    {
        int pipe_hd[2]; pipe(pipe_hd);
        char *line;
        while ((line = readline("> ")))
        {
            if (strcmp(line, cmd->heredoc) == 0) { free(line); break; }
            write(pipe_hd[1], line, strlen(line));
            write(pipe_hd[1], "\n", 1);
            free(line);
        }
        close(pipe_hd[1]);
        dup2(pipe_hd[0], STDIN_FILENO);
        close(pipe_hd[0]);
    }

    return 0;
}

// execute a single command or handle redirection-only like bash
void execute_command(t_command *cmd, char **env)
{
    // Expand variables
    for (int i = 0; cmd->args && cmd->args[i]; i++)
    {
        char *expanded = expand_variables(cmd->args[i], env);
        free(cmd->args[i]);
        cmd->args[i] = expanded;
    }

    // if no command, only redirections/heredoc
    if (!cmd->args || !cmd->args[0])
    {
        int ret = apply_redirections_only(cmd);
        exit(ret);
    }
    // builtin check
    if (is_builtin(cmd->args[0]))
    {
        int code = run_builtin(cmd, &env);
        exit(code);
    }

    // normal redirections (stdin, stdout, stderr, heredoc)
    if (cmd->infile)
    {
        int fd = open(cmd->infile, O_RDONLY);
        if (fd < 0) { perror(cmd->infile); exit(1); }
        dup2(fd, STDIN_FILENO); close(fd);
    }

    if (cmd->outfile)
    {
        int fd = cmd->append ? open(cmd->outfile, O_CREAT|O_WRONLY|O_APPEND,0644)
                             : open(cmd->outfile, O_CREAT|O_WRONLY|O_TRUNC,0644);
        if (fd < 0) { perror(cmd->outfile); exit(1); }
        dup2(fd, STDOUT_FILENO); close(fd);
    }

    if (cmd->errfile)
    {
        int fd = cmd->err_append ? open(cmd->errfile, O_CREAT|O_WRONLY|O_APPEND,0644)
                                 : open(cmd->errfile, O_CREAT|O_WRONLY|O_TRUNC,0644);
        if (fd < 0) { perror(cmd->errfile); exit(1); }
        dup2(fd, STDERR_FILENO); close(fd);
    }

    if (cmd->heredoc)
    {
        int pipe_hd[2]; pipe(pipe_hd);
        char *line;
        while ((line = readline("> ")))
        {
            if (strcmp(line, cmd->heredoc) == 0) { free(line); break; }
            write(pipe_hd[1], line, strlen(line));
            write(pipe_hd[1], "\n", 1);
            free(line);
        }
        close(pipe_hd[1]);
        dup2(pipe_hd[0], STDIN_FILENO);
        close(pipe_hd[0]);
    }

    // execute external command
    char *path = build_path(env, cmd->args[0]);
    if (path) execve(path, cmd->args, env);
    perror(cmd->args[0]);
    exit(127); // bash uses 127 for command not found
}

int execute_pipeline(t_command *cmd, char **env)
{
    int fd_in = 0;
    int last_status = 0;
    pid_t last_pid = 0;
    while (cmd)
    {
        int pipefd[2];
        if (cmd->next)
            pipe(pipefd);
        pid_t pid = fork();
        if (pid == 0)
        {
            if (fd_in != 0) { dup2(fd_in, STDIN_FILENO); close(fd_in); }
            if (cmd->next) { close(pipefd[0]); dup2(pipefd[1], STDOUT_FILENO); close(pipefd[1]); }
            if (cmd->infile) { int fd = open(cmd->infile, O_RDONLY); dup2(fd, STDIN_FILENO); close(fd); }
            if (cmd->outfile) { int fd = cmd->append ? open(cmd->outfile, O_CREAT|O_WRONLY|O_APPEND,0644) : open(cmd->outfile, O_CREAT|O_WRONLY|O_TRUNC,0644); dup2(fd, STDOUT_FILENO); close(fd); }
            if (cmd->errfile) { int fd = cmd->err_append ? open(cmd->errfile,O_CREAT|O_WRONLY|O_APPEND,0644) : open(cmd->errfile,O_CREAT|O_WRONLY|O_TRUNC,0644); dup2(fd, STDERR_FILENO); close(fd); }
            if (cmd->heredoc)
            {
                int hd_pipe[2]; pipe(hd_pipe);
                char *line;
                while ((line = readline("> ")))
                {
                    if (strcmp(line, cmd->heredoc) == 0) { free(line); break; }
                    write(hd_pipe[1], line, strlen(line));
                    write(hd_pipe[1], "\n", 1);
                    free(line);
                }
                close(hd_pipe[1]);
                dup2(hd_pipe[0], STDIN_FILENO);
                close(hd_pipe[0]);
            }
            char *path = build_path(env, cmd->args[0]);
            if (path) execve(path, cmd->args, env);
            perror(cmd->args[0]);
            exit(127);
        }
        else
        {
            last_pid = pid;
            if (fd_in != 0) close(fd_in);
            if (cmd->next) { close(pipefd[1]); fd_in = pipefd[0]; }
        }
        cmd = cmd->next;
    }
    int status;
    waitpid(last_pid, &status, 0);
    last_status = WIFEXITED(status) ? WEXITSTATUS(status) : 1;
    while (wait(NULL) > 0);
    return last_status;
}


t_job *parse_jobs(char **tokens)
{
    t_job *head = NULL, *curr_job = NULL;
    int i = 0;
    while (tokens[i])
    {
        t_job *job = calloc(1, sizeof(t_job));
        char *job_tokens[256];
        int j = 0;
        while (tokens[i] && strcmp(tokens[i], "&&") != 0 && strcmp(tokens[i], "||") != 0)
            job_tokens[j++] = tokens[i++];
        job_tokens[j] = NULL;
        job->cmds = parse_commands(job_tokens);
        if (tokens[i])
        {
            if (strcmp(tokens[i], "&&") == 0) job->is_and = 1;
            else if (strcmp(tokens[i], "||") == 0) job->is_or = 1;
            i++;
        }
        if (!head) head = job;
        if (curr_job) curr_job->next = job;
        curr_job = job;
    }
    return head;
}

int execute_jobs(t_job *jobs, char **env)
{
    int last_status = 0;
    while (jobs)
    {
        // skip based on operator
        if ((jobs->is_and && last_status != 0) || 
            (jobs->is_or && last_status == 0))
        {
            jobs = jobs->next;
            continue;
        }
        last_status = execute_pipeline(jobs->cmds, env);
        jobs = jobs->next;
    }
    return last_status;
}
