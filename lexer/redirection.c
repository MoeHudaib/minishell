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

void execute_command(t_command *cmd, char **env)
{
    for (int i = 0; cmd->args[i]; i++)
    {
        char *expanded = expand_variables(cmd->args[i], env);
        free(cmd->args[i]);
        cmd->args[i] = expanded;
    }
    if (is_builtin(cmd->args[0]))
    {
        int code = run_builtin(cmd, &env);
        exit(code); // use returned status
    }
    if (cmd->infile)
    {
        int fd = open(cmd->infile, O_RDONLY);
        if (fd < 0) perror(cmd->infile);
        else { dup2(fd, STDIN_FILENO); close(fd); }
    }

    if (cmd->outfile)
    {
        int fd = cmd->append ? open(cmd->outfile, O_CREAT|O_WRONLY|O_APPEND,0644)
                             : open(cmd->outfile, O_CREAT|O_WRONLY|O_TRUNC,0644);
        if (fd < 0) perror(cmd->outfile);
        else { dup2(fd, STDOUT_FILENO); close(fd); }
    }
    if (cmd->errfile)
    {
        int fd = cmd->err_append ? 
                open(cmd->errfile, O_CREAT|O_WRONLY|O_APPEND, 0777) :
                open(cmd->errfile, O_CREAT|O_WRONLY|O_TRUNC, 0777);
        if (fd < 0) perror(cmd->errfile);
        else
        {
            dup2(fd, STDERR_FILENO); // redirect stderr
            close(fd);
        }
    }
    if (cmd->heredoc)
    {
        int pipe_hd[2]; pipe(pipe_hd);
        char *line;
        while ((line = readline("> ")))
        {
            if (strcmp(line, cmd->heredoc) == 0) { free(line); break; }
            write(pipe_hd[1], line, strlen(line)); write(pipe_hd[1], "\n", 1); free(line);
        }
        close(pipe_hd[1]);
        dup2(pipe_hd[0], STDIN_FILENO);
    }
    char *path = build_path(env, cmd->args[0]);
    if (path) execve(path, cmd->args, env);
    perror("command not found");
    exit(1);
}

void execute_pipeline(t_command *cmd, char **env)
{
    int fd_in = 0; // stdin for the first command
    while (cmd)
    {
        int pipefd[2];
        if (cmd->next)
            pipe(pipefd); // create pipe for next command
        pid_t pid = fork();
        if (pid == 0) // child
        {
            if (fd_in != 0) { dup2(fd_in, STDIN_FILENO); close(fd_in); }
            if (cmd->next)
            {
                close(pipefd[0]);
                dup2(pipefd[1], STDOUT_FILENO);
                close(pipefd[1]);
            }
            if (cmd->infile)
            {
                int fd = open(cmd->infile, O_RDONLY);
                if (fd < 0) perror(cmd->infile);
                else { dup2(fd, STDIN_FILENO); close(fd); }
            }
            if (cmd->outfile)
            {
                int fd;
                if (cmd->append)
                    fd = open(cmd->outfile, O_CREAT | O_WRONLY | O_APPEND, 0644);
                else
                    fd = open(cmd->outfile, O_CREAT | O_WRONLY | O_TRUNC, 0644);
                if (fd < 0) perror(cmd->outfile);
                else { dup2(fd, STDOUT_FILENO); close(fd); }
            }
            if (cmd->heredoc)
            {
                int pipe_hd[2];
                pipe(pipe_hd);
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
            }
            char *path = build_path(env, cmd->args[0]);
            if (path) execve(path, cmd->args, env);
            perror("command not found");
            exit(1);
        }
        else
        {
            wait(NULL);
            if (fd_in != 0) close(fd_in);
            if (cmd->next) close(pipefd[1]);
            fd_in = pipefd[0]; // next command reads from previous pipe
        }
        cmd = cmd->next;
    }
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
    t_job *prev = NULL;

    while (jobs)
    {
        if (prev)
        {
            if (prev->is_and && last_status != 0)
            {
                jobs = jobs->next;
                prev = prev ? prev->next : NULL;
                continue;
            }
            else if (prev->is_or && last_status == 0)
            {
                jobs = jobs->next;
                prev = prev ? prev->next : NULL;
                continue;
            }
        }
        execute_pipeline(jobs->cmds, env);
        int status = 0;
        wait(&status);
        last_status = WIFEXITED(status) ? WEXITSTATUS(status) : 1;
        prev = jobs;
        jobs = jobs->next;
    }

    return last_status;
}