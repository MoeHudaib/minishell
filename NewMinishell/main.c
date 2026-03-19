/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mohammad <mohammad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/19 06:28:50 by mohammad          #+#    #+#             */
/*   Updated: 2026/03/19 06:33:59 by mohammad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "organize.h"

static int	handle_signal_or_empty(char *line, int last_status)
{
	if (g_sig == SIGINT)
		return (free(line), 130);
	if (*line == '\0')
		return (free(line), last_status);
	return (-1);
}

static int	process_line(char *line, t_env **env_list, int last_status)
{
	t_lexer	*tokens;
	int		status;

	add_history(line);
	tokens = lex_line(line);
	free(line);
	if (!tokens)
		return (last_status);
	expand_lexer_tokens(tokens, last_status, *env_list);
	status = are_we_gonna_split(tokens, env_list, last_status);
	signals_interactive();
	delete_lexer(&tokens);
	return (status);
}

static char	*get_line(t_env *env_list)
{
	char	*prompt;
	char	*line;

	prompt = build_prompt(env_list);
	line = read_full_input(prompt);
	free(prompt);
	return (line);
}

static int	shell_loop(t_env *env_list)
{
	char	*line;
	int		last_status;
	int		sig_check;

	last_status = 0;
	while (1)
	{
		g_sig = 0;
		line = get_line(env_list);
		if (!line)
			return (write(STDOUT_FILENO, "exit\n", 5), last_status);
		sig_check = handle_signal_or_empty(line, last_status);
		if (sig_check != -1)
		{
			last_status = sig_check;
			continue ;
		}
		last_status = process_line(line, &env_list, last_status);
	}
}

int	main(int ac, char **av, char **env)
{
	t_env	*env_list;

	(void)ac;
	(void)av;
	env_list = set_env(env);
	signals_interactive();
	return (shell_loop(env_list));
}
