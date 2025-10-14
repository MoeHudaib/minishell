#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include "./parse/parse.h"
#include "./lexer/quotes.h"
#include <sys/wait.h>
#include <signal.h>

void handle_sigint(int sig)
{
    (void)sig;
    write(1, "\nCaught SIGINT (Ctrl+C)\n", 25);
}

static size_t	count_sub(const char *str, const char *sub)
{
	size_t	count = 0;
	size_t	sublen = strlen(sub);

	if (!*sub)
		return (0);
	while ((str = strstr(str, sub)))
	{
		count++;
		str += sublen;
	}
	return (count);
}

char	*ft_strreplace(const char *str, const char *sub, const char *replace)
{
	size_t	sub_len = strlen(sub);
	size_t	rep_len = strlen(replace);
	size_t	count = count_sub(str, sub);
	size_t	new_len = strlen(str) + count * (rep_len - sub_len);
	char	*result = malloc(new_len + 1);
	char	*p = result;
	const char *pos;

	if (!result)
		return (NULL);
	while ((pos = strstr(str, sub)))
	{
		size_t	before = pos - str;
		memcpy(p, str, before);
		p += before;
		memcpy(p, replace, rep_len);
		p += rep_len;
		str = pos + sub_len;
	}
	strcpy(p, str);
	return (result);
}

int main(int ac, char **av, char **env)
{
    char cwd[1024];
    getcwd(cwd, sizeof(cwd));
    char *line;

    signal(SIGINT, handle_sigint);

    while (1)
    {
        line = read_full_input(cwd);
        if (!line || !*line) { free(line); break; }

        add_history(line);

        char **tokens = split_with_quotes(line);

        t_job *jobs = parse_jobs(tokens);

        execute_jobs(jobs, env);

        // free everything
        t_job *tmp_job;
        while (jobs)
        {
            tmp_job = jobs->next;

            t_command *cmd = jobs->cmds;
            t_command *tmp_cmd;
            while (cmd)
            {
                tmp_cmd = cmd->next;
                for (int i = 0; cmd->args[i]; i++) free(cmd->args[i]);
                free(cmd->args);
                if (cmd->infile) free(cmd->infile);
                if (cmd->outfile) free(cmd->outfile);
                if (cmd->heredoc) free(cmd->heredoc);
                free(cmd);
                cmd = tmp_cmd;
            }

            free(jobs);
            jobs = tmp_job;
        }

        for (int i = 0; tokens[i]; i++) free(tokens[i]);
        free(tokens);
        free(line);
    }
}
