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
# include "libs/libft/libft.h"

/*
** ============================================================
** LAYER 1 — ENV
** no dependencies on your own structs
** ============================================================
*/
typedef struct	s_env
{
	char			*key;
	char			*value;
	struct s_env	*next;
}	t_env;

t_env	*set_env(char **env);
t_env	*seperate_key_value(char *line, t_env *new_one);
t_env	*add_last(t_env **head, t_env *node);
t_env	*update_env(char *key, char *value, t_env **head);
char	**env_to_array(t_env *head);
char	*get_env_value(char *key, char **env);
int		print_list(t_env *head);
void	envclear(t_env **head);
int		ft_export(t_env **head, char **cmd);
int		ft_unset(t_env **head, char **cmd);


/*
** ============================================================
** LAYER 2 — LEXER
** depends on: t_env
** ============================================================
*/
typedef enum e_decider
{
	YES = 1,
	NO = 0
}	t_decider;

typedef enum e_token_type
{
	TOKEN_WORD,		// echo, hello, filename
	TOKEN_PIPE,		// |
	TOKEN_REDIR_IN,	// <
	TOKEN_REDIR_OUT,	// >
	TOKEN_REDIR_APPEND,// >>
	TOKEN_HEREDOC,	 // <<
	TOKEN_AND,		 // &&
	TOKEN_OR,		  // ||
	TOKEN_EOF
}	t_token_type;

typedef struct s_lexer
{
	char			*token;
	t_token_type	type;
	int			 single_quote;
	int			 double_quote;
	struct s_lexer	*next;
}	t_lexer;

char	*read_full_input(char *str);
char	**split_with_quotes(const char *str, t_token_type *types);
int		has_unclosed_quotes(const char *s);

t_lexer	*lexer_init(char *data, t_token_type type);
t_lexer	*add_new_token_back(t_lexer **lexer_head, char *data, t_token_type type);
t_lexer	*add_token_back(t_lexer **lexer_head, t_lexer *lexer);
void	*free_tokens(char **tokens, char *line, t_lexer **lexer_head);
t_lexer	*lex_line(char *line);
void	delete_lexer(t_lexer **head);
void	print_tokens(t_lexer *lexer_head);
char	**organize(t_lexer *tokens);

/*
** ============================================================
** LAYER 3 — EXPANDER
** depends on: t_lexer, t_env
** ============================================================
*/

int		is_valid_var_char(char c);
void	expand_lexer_tokens(t_lexer *tokens, int last_status, t_env *env);
char	*expand_token(char *token, int last_status, char **env);
char	*expand_dollar(const char *str, int *i, int last_status, char **env);
char	*get_exit_status(int status);

/*
** ============================================================
** LAYER 4 — PARSE
** depends on: t_lexer, t_env
** ============================================================
*/
typedef struct s_redir
{
	t_token_type	type;	// REDIR_IN, REDIR_OUT, APPEND, HEREDOC
	char			*file;	// filename or heredoc delimiter
	int			 fd;		 // heredoc pipe read end
	int			 expand;
	struct s_redir	*next;
}	t_redir;

typedef struct s_cmd
{
	char			**args;	// ["echo", "hello", NULL]
	t_redir		 *redirs;	// linked list of redirections
	struct s_cmd	*next;	// next command after pipe
}	t_cmd;

// So cmd1 | cmd2 > outfile becomes:
// t_cmd[0]: args=["cmd1"]		redirs=NULL
// t_cmd[1]: args=["cmd2"]		redirs=[REDIR_OUT -> "outfile"]

char	*try_path(char *cmd, char *path);
char	*return_path(t_env *head);
char	*free_enp(char **enp);
char	*build_path(t_env **head, char *cmd);


t_cmd	*parse(t_lexer *tokens);
int		has_complex(t_lexer *tokens);
int		is_redir(t_token_type type);
int		apply_redirections(t_cmd *cmd);
void	add_arg(t_cmd *cmd, char *arg);
void	add_redir(t_cmd *cmd, t_token_type type, char *file);
void	append_cmd(t_cmd **head, t_cmd *cmd);
int		are_we_gonna_split(t_lexer *tokens, t_env **env, int last_status);
void	free_cmd_list(t_cmd *cmd_list);
int		prepare_heredocs(t_cmd *cmd_list);

/*
** ============================================================
** LAYER 5 — BUILTINS
** depends on: t_env
** ============================================================
*/

int		ft_pwd(void);
int		ft_exit(char **cmd, int last_status);
int		ft_env(t_env *env);
int		ft_echo(char **cmd);
int		ft_cd(char **cmd, t_env **env);
int		is_builtin(char *cmd);
int		exec_builtin(char **cmd, t_env **env, int last_status);
int		ft_isnumeric(char *str);

/*
** ============================================================
** LAYER 6 — EXECUTOR
** depends on: t_env, builtins, parse
** ============================================================
*/

void	execute_command(char **cmd, t_env **env, int last_status);
int		work(int len_of_exe, t_cmd *cmd_list, t_env **env, int last_status);
int		create_processes(int len_of_exe, int fd[len_of_exe - 1][2], t_cmd *cmd_list, t_env **env, pid_t *pids, int last_status);


/*
** ============================================================
** LAYER 7 — SIGNALS
** depends on: nothing (uses only globals)
** ============================================================
*/
// The ONE allowed global: only stores the signal number, nothing else.
extern volatile sig_atomic_t	g_sig;

void	signals_child(void);
void	signals_reset_child(void);
void	signals_heredoc(void);
void	signals_interactive(void);

#endif

// **Dependency order visualized:**
// libft
//	 └── t_env  (layer 1)
//		 ├── t_lexer	  (layer 2)
//		 │	 ├── expander   (layer 3)
//		 │	 └── t_parse	(layer 4)
//		 └── builtins	 (layer 5)
//			   └── executor   (layer 6)	
// g_signal / signals		 (layer 7 — standalone)