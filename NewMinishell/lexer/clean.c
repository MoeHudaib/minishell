#include "../organize.h"

void	*free_tokens(char **tokens, char *line, t_lexer **lexer_head)
{
	int	i;

	if (tokens)
	{
		i = 0;
		while (tokens[i])
		{
			free(tokens[i]);
			i++;
		}
		free(tokens);
	}
	if (line)
		free(line);
	if (lexer_head)
		delete_lexer(lexer_head);
	return (NULL);
}

static void free_redirs(t_redir *redir)
{
	t_redir *tmp;

	while (redir)
	{
		tmp = redir->next;
		if (redir->type == TOKEN_HEREDOC && redir->fd >= 0)
			close(redir->fd);		   // close pipe read-end if never consumed
		free(redir->file);
		free(redir);
		redir = tmp;
	}
}

static void free_args(char **args)
{
	int i;

	if (!args)
		return ;
	i = 0;
	while (args[i])
	{
		free(args[i]);
		i++;
	}
	free(args);
}

void free_cmd_list(t_cmd *cmd_list)
{
	t_cmd *tmp;

	while (cmd_list)
	{
		tmp = cmd_list->next;
		free_args(cmd_list->args);
		free_redirs(cmd_list->redirs);
		free(cmd_list);
		cmd_list = tmp;
	}
}