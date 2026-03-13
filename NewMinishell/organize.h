#ifndef ORGANIZE_H
# define ORGANIZE_H

/*
** ============================================================
** SYSTEM INCLUDES
** ============================================================
*/
# include <unistd.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <fcntl.h>
# include <ctype.h>
# include <signal.h>
# include <sys/wait.h>
# include <readline/readline.h>
# include <readline/history.h>

/*
** ============================================================
** LIBFT
** ============================================================
*/
# include "../libs/libft/libft.h"

/*
** ============================================================
** LAYER 1 — ENV
** no dependencies on your own structs
** ============================================================
*/
typedef struct s_env
{
    char            *key;
    char            *value;
    struct s_env    *next;
}   t_env;

t_env   *set_env(char **env);
t_env   *seperate_key_value(char *line, t_env *new_one);
t_env   *add_last(t_env **head, t_env *node);
int     print_list(t_env *head);
void    envclear(t_env **head);
int     ft_unset(t_env **head, char *key);
int     ft_export(t_env **head, char *line);
char    **env_to_array(t_env *head);
char    *get_env_value(t_env *env_list, char *key);

/*
** ============================================================
** LAYER 2 — LEXER
** depends on: t_env
** ============================================================
*/
typedef enum e_decider
{
    YES = 1,
    NO  = 0
}   t_decider;

typedef struct s_lexer
{
    char            *token;
    int             single_quote;
    int             double_quote;
    struct s_lexer  *next;
}   t_lexer;

char    **split_with_quotes(const char *str);
t_lexer *lexer_init(char *data);
t_lexer *add_new_token_front(t_lexer **lexer_head, char *data);
t_lexer *add_new_token_back(t_lexer **lexer_head, char *data);
t_lexer *add_token_back(t_lexer **lexer_head, t_lexer *lexer);
void    delete_lexer(t_lexer **head);
void    *free_tokens(char **tokens, char *line, t_lexer **lexer_head);
char    *read_full_input(char *str);
t_lexer *lex_line(char *line);
void    print_tokens(t_lexer *lexer_head);
int     is_it_simple_exe(char *line);
int     has_unclosed_quotes(const char *s);
char    **organize(t_lexer *tokens);

/*
** ============================================================
** LAYER 3 — EXPANDER
** depends on: t_lexer, t_env
** ============================================================
*/
void    expand_lexer_tokens(t_lexer *tokens, int last_status, t_env *env_list);

/*
** ============================================================
** LAYER 4 — PARSE
** depends on: t_lexer, t_env
** ============================================================
*/
typedef struct s_parse // Why don't we create a linked list of lexer tokens linked lists
{
    char        **cmd;
    t_env       *next;
}   t_parse;

char    *try_path(char *cmd, char *path);
char    *return_path(t_env *head);
char    *free_enp(char **enp);
char    *build_path(t_env **head, char *cmd);
int     has_complex(t_lexer *tokens);
t_parse *split_into_processes(t_lexer *head);
int     are_we_gonna_split(t_lexer *head, t_env **env);

/*
** ============================================================
** LAYER 5 — BUILTINS
** depends on: t_env
** ============================================================
*/
int     ft_pwd(void);
int     ft_exit(char **cmd);
int     ft_env(t_env *env);
int     ft_echo(char **cmd);
int     ft_cd(char **cmd, t_env **env);
int     is_builtin(char *cmd);
int     exec_builtin(char **cmd, t_env **env);

/*
** ============================================================
** LAYER 6 — EXECUTOR
** depends on: t_env, builtins, parse
** ============================================================
*/
typedef struct s_exe
{
    char    **cmd;
    t_env   *head;
}   t_exe;

void    execute_command(char **cmd, t_env **env);
int     create_processes(int len_of_exe, int fd[len_of_exe - 1][2],
            char ***cmds, t_env **env);
int     work(int len_of_exe, char ***cmds, t_env **env);

/*
** ============================================================
** LAYER 7 — SIGNALS
** depends on: nothing (uses only globals)
** ============================================================
*/
extern int  g_signal;

void    setup_signals(void);

#endif

// **Dependency order visualized:**
// libft
//   └── t_env  (layer 1)
//         ├── t_lexer      (layer 2)
//         │     ├── expander   (layer 3)
//         │     └── t_parse    (layer 4)
//         └── builtins     (layer 5)
//               └── executor   (layer 6)
                            
// g_signal / signals         (layer 7 — standalone)