#ifndef QUOTES_H
# define QUOTES_H

# include <stdio.h>
# include <unistd.h>
# include <stdlib.h>
# include <string.h>
# include <sys/wait.h>
# include "../libs/libft/libft.h"
# include "../parse/parse.h"
# include <signal.h>
# include <readline/readline.h>
# include <readline/history.h>

typedef struct s_command {
    char **args;
    char *infile;
    char *outfile;
    int append;
    char *heredoc;
    char *errfile;
    int err_append;
    struct s_command *next;
} t_command;

typedef struct s_job {
    t_command *cmds;
    int is_and;
    int is_or;
    struct s_job *next;
} t_job;

char	*read_full_input(char *str);
char **split_with_quotes(const char *str);
char **tokenize(char *str);
t_command *parse_commands(char **tokens);
int execute_pipeline(t_command *cmd, char **env);
void execute_command(t_command *cmd, char **env);
t_job *parse_jobs(char **tokens);
int execute_jobs(t_job *jobs, char **env);
char *expand_variables(char *input, char **env);
int run_builtin(t_command *cmd, char ***envp);
int is_builtin(const char *cmd);

#endif