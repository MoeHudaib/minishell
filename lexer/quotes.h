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
    char **args;        // command + args
    char *infile;       // stdin '<'
    char *outfile;      // stdout '>' or '>>'
    int append;         // 1 if '>>'
    char *heredoc;      // '<<'
    char *errfile;      // stderr '2>' or '2>>'
    int err_append;     // 1 if '2>>'
    struct s_command *next; // next command in a pipe
} t_command;


typedef struct s_job {
    t_command *cmds;   // commands in a pipeline 
    int is_and;        // 1 if chained with &&
    int is_or;         // 1 if chained with ||
    struct s_job *next; // next job in the command line
} t_job;

char	*read_full_input(char *str);
char **split_with_quotes(const char *str);
int has_quote(char *str);
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