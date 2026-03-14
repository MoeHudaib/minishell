#include "../organize.h"

t_cmd *new_cmd(void)
{
    t_cmd *cmd;

    cmd = malloc(sizeof(t_cmd));
    if (!cmd)
        return (NULL);
    cmd->args = NULL;
    cmd->redirs = NULL;
    cmd->next = NULL;
    return (cmd);
}

void add_arg(t_cmd *cmd, char *arg)
{
    char    **new_args;
    int     i;

    i = 0;
    if (cmd->args)
        while (cmd->args[i])
            i++;
    new_args = malloc(sizeof(char *) * (i + 2));
    if (!new_args)
        return ;
    i = 0;
    if (cmd->args)
    {
        while (cmd->args[i])
        {
            new_args[i] = cmd->args[i];
            i++;
        }
        free(cmd->args);
    }
    new_args[i] = ft_strdup(arg);
    new_args[i + 1] = NULL;
    cmd->args = new_args;
}

void append_cmd(t_cmd **head, t_cmd *cmd)
{
    t_cmd *current;

    if (!*head)
    {
        *head = cmd;
        return ;
    }
    current = *head;
    while (current->next)
        current = current->next;
    current->next = cmd;
}

int is_redir(t_token_type type)
{
    return (type == TOKEN_REDIR_IN ||
            type == TOKEN_REDIR_OUT ||
            type == TOKEN_REDIR_APPEND ||
            type == TOKEN_HEREDOC);
}

static t_redir *new_redir(t_token_type type, char *file)
{
    t_redir *redir;

    redir = malloc(sizeof(t_redir));
    if (!redir)
        return (NULL);
    redir->type = type;
    redir->file = ft_strdup(file);
    redir->fd = -1;      // ← fix: safe sentinel, not garbage
    redir->expand = 0;
    redir->next = NULL;
    return (redir);
}

void add_redir(t_cmd *cmd, t_token_type type, char *file)
{
    t_redir *new;
    t_redir *current;

    new = new_redir(type, file);
    if (!new)
        return ;
    if (!cmd->redirs)
    {
        cmd->redirs = new;
        return ;
    }
    current = cmd->redirs;
    while (current->next)
        current = current->next;
    current->next = new;
}

t_cmd *parse(t_lexer *tokens)
{
    t_cmd        *head = NULL;
    t_cmd        *current = new_cmd();
    t_token_type redir_type;

    while (tokens)
    {
        if (tokens->type == TOKEN_WORD)
            add_arg(current, tokens->token);
        else if (tokens->type == TOKEN_PIPE)
        {
            append_cmd(&head, current);
            current = new_cmd();
        }
        else if (is_redir(tokens->type))
        {
            redir_type = tokens->type;
            tokens = tokens->next;
            if (!tokens)
            {
                ft_putendl_fd("minishell: syntax error near unexpected token `newline'", STDERR_FILENO);
                free_cmd_list(head);
                free(current);
                return (NULL);
            }
            if (tokens->type != TOKEN_WORD)
            {
                ft_putstr_fd("minishell: syntax error near unexpected token `", STDERR_FILENO);
                ft_putstr_fd(tokens->token, STDERR_FILENO);
                ft_putendl_fd("'", STDERR_FILENO);
                free_cmd_list(head);
                free(current);
                return (NULL);
            }
            add_redir(current, redir_type, tokens->token);
        }
        tokens = tokens->next;
    }
    append_cmd(&head, current);
    return (head);
}