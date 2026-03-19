/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   organize.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mohammad <mohammad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/19 06:16:37 by mohammad          #+#    #+#             */
/*   Updated: 2026/03/19 06:33:49 by mohammad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
** LIBFT + SUBMODULE HEADERS
** ============================================================
*/
# include "libs/libft/libft.h"
# include "exe/work.h"

/*
** ============================================================
** LAYER 1 — ENV
** no dependencies on your own structs
** ============================================================
*/
typedef struct s_env
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
	TOKEN_WORD,
	TOKEN_PIPE,
	TOKEN_REDIR_IN,
	TOKEN_REDIR_OUT,
	TOKEN_REDIR_APPEND,
	TOKEN_HEREDOC,
	TOKEN_AND,
	TOKEN_OR,
	TOKEN_EOF
}	t_token_type;

typedef struct s_lexer
{
	char			*token;
	t_token_type	type;
	int				single_quote;
	int				double_quote;
	struct s_lexer	*next;
}	t_lexer;

typedef struct s_split_ctx
{
	char			**result;
	t_token_type	*types;
	int				count;
}	t_split_ctx;

char	*read_full_input(char *str);
char	**split_with_quotes(const char *str, t_token_type *types);
int		has_unclosed_quotes(const char *s);
t_lexer	*lexer_init(char *data, t_token_type type);
t_lexer	*add_new_token_back(t_lexer **lexer_head,
			char *data, t_token_type type);
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
typedef struct s_expand
{
	char	*token;
	int		*i;
	int		last_status;
	char	**env;
	char	q;
}	t_expand;

int		is_valid_var_char(char c);
char	*get_exit_status(int status);
char	*get_env_value(char *key, char **env);
char	*ft_strjoin_char(char *s, char c);
void	handle_single_quote(int *i, char *quote);
void	handle_double_quote(int *i, char *quote);
void	expand_lexer_tokens(t_lexer *tokens, int last_status, t_env *env);
char	*expand_token(char *token, int last_status, char **env);
char	*expand_dollar(const char *str, int *i, int last_status, char **env);

/*
** ============================================================
** LAYER 4 — PARSE
** depends on: t_lexer, t_env
** ============================================================
*/
typedef struct s_redir
{
	t_token_type	type;
	char			*file;
	int				fd;
	int				expand;
	struct s_redir	*next;
}	t_redir;

typedef struct s_cmd
{
	char			**args;
	t_redir			*redirs;
	struct s_cmd	*next;
}	t_cmd;

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
int		is_parent_builtin(char *cmd);

/*
** ============================================================
** LAYER 7 — SIGNALS
** depends on: nothing (uses only globals)
** ============================================================
*/
extern volatile sig_atomic_t	g_sig;

void	signals_child(void);
void	signals_reset_child(void);
void	signals_heredoc(void);
void	signals_interactive(void);

/*
** ============================================================
** LAYER 8 — MAIN
** depends on: everything
** ============================================================
*/

char	*build_prompt(t_env *env_list);

#endif