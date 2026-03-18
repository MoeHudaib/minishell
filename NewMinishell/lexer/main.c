#include "../organize.h"

static char *build_prompt(t_env *env_list)
{
	char *cwd;
	char *prompt;

	cwd = NULL;
	if (!cwd)
		return (ft_strdup("minishell$ "));
	prompt = ft_strjoin(cwd, " minishell$ ");   // no "> " suffix
	return (prompt);
}

int main(int ac, char **av, char **env) // WE NNED TO HANDLE THE EXIT STATUS ASAP
{
	char	*line;
	char	*prompt;
	t_lexer *tokens;
	t_env   *env_list;
	int	 last_status;

	(void)ac;
	(void)av;
	last_status = 0;
	env_list = set_env(env);

	// Set interactive mode signals once before entering the loop
	signals_interactive();

	while (1)
	{
		g_sig = 0;						  // clear before every readline call
		prompt = build_prompt(env_list);
		line = read_full_input(prompt);	 // readline blocks here
		free(prompt);

		// Ctrl+D → readline returns NULL → clean exit
		if (!line)
		{
			write(STDOUT_FILENO, "exit\n", 5);
			break ;
		}

		// Ctrl+C during readline → g_sig == SIGINT, line is empty string
		if (g_sig == SIGINT)
		{
			free(line);
			last_status = 130;			  // bash sets $? = 130 on SIGINT
			continue ;
		}

		if (*line == '\0')
		{
			free(line);
			continue ;
		}

		add_history(line);
		tokens = lex_line(line);
		free(line);
		if (!tokens)
			continue ;

		expand_lexer_tokens(tokens, last_status, env_list);

		// signals_child() / signals_reset_child() are called inside
		// are_we_gonna_split → your executor, around fork/waitpid
		last_status = are_we_gonna_split(tokens, &env_list, last_status);

		// After child finishes, re-arm interactive handlers
		signals_interactive();

		delete_lexer(&tokens);
	}
	return (last_status);
}