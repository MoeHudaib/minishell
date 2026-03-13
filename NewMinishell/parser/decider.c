#include "../organize.h"

/* counts tokens until pipe or end */
static int  count_until_pipe(t_lexer *tokens)
{
    int     count;

    count = 0;
    while (tokens && strncmp(tokens->token, "|", 2) != 0)
    {
        count++;
        tokens = tokens->next;
    }
    return (count);
}

/* counts how many pipes = how many commands */
static int  count_commands(t_lexer *tokens)
{
    int     count;

    count = 1;
    while (tokens)
    {
        if (strncmp(tokens->token, "|", 2) == 0)
            count++;
        tokens = tokens->next;
    }
    return (count);
}

/* apply redirections for a command before execve */
static int  apply_redirections(t_lexer *tokens)
{
    int fd;

    while (tokens && strncmp(tokens->token, "|", 2) != 0)
    {
        if (strncmp(tokens->token, ">", 2) == 0 && tokens->next)
        {
            fd = open(tokens->next->token, O_WRONLY | O_CREAT | O_TRUNC, 0644);
            if (fd < 0)
                return (1);
            dup2(fd, STDOUT_FILENO);
            close(fd);
            tokens = tokens->next;
        }
        else if (strncmp(tokens->token, ">>", 3) == 0 && tokens->next)
        {
            fd = open(tokens->next->token, O_WRONLY | O_CREAT | O_APPEND, 0644);
            if (fd < 0)
                return (1);
            dup2(fd, STDOUT_FILENO);
            close(fd);
            tokens = tokens->next;
        }
        else if (strncmp(tokens->token, "<", 2) == 0 && tokens->next)
        {
            fd = open(tokens->next->token, O_RDONLY);
            if (fd < 0)
                return (1);
            dup2(fd, STDIN_FILENO);
            close(fd);
            tokens = tokens->next;
        }
        else if (strncmp(tokens->token, "<<", 3) == 0 && tokens->next)
        {
            // heredoc — handled separately, skip for now
            tokens = tokens->next;
        }
        tokens = tokens->next;
    }
    return (0);
}

/* builds char** from tokens, skipping redirection tokens and their targets */
char    **organize(t_lexer *tokens)
{
    char        **cmd;
    int         count;
    int         i;
    t_lexer     *tmp;

    count = 0;
    tmp = tokens;
    while (tmp && strncmp(tmp->token, "|", 2) != 0)
    {
        if (strncmp(tmp->token, ">", 2) == 0
            || strncmp(tmp->token, ">>", 3) == 0
            || strncmp(tmp->token, "<", 2) == 0
            || strncmp(tmp->token, "<<", 3) == 0)
        {
            tmp = tmp->next; // skip redirection symbol
            if (tmp)
                tmp = tmp->next; // skip redirection target
            continue ;
        }
        count++;
        tmp = tmp->next;
    }
    cmd = malloc(sizeof(char *) * (count + 1));
    if (!cmd)
        return (NULL);
    i = 0;
    while (tokens && strncmp(tokens->token, "|", 2) != 0)
    {
        if (strncmp(tokens->token, ">", 2) == 0
            || strncmp(tokens->token, ">>", 3) == 0
            || strncmp(tokens->token, "<", 2) == 0
            || strncmp(tokens->token, "<<", 3) == 0)
        {
            tokens = tokens->next;
            if (tokens)
                tokens = tokens->next;
            continue ;
        }
        cmd[i++] = ft_strdup(tokens->token);
        tokens = tokens->next;
    }
    cmd[i] = NULL;
    return (cmd);
}

/* splits t_lexer on pipes, builds t_parse linked list */
t_parse *split_into_processes(t_lexer *head)
{
    t_parse     *parse_list;
    t_parse     *current;
    t_parse     *new_node;
    t_lexer     *tmp;

    if (!head)
        return (NULL);
    parse_list = NULL;
    current = NULL;
    tmp = head;
    while (tmp)
    {
        new_node = malloc(sizeof(t_parse));
        if (!new_node)
            return (NULL);
        new_node->cmd = organize(tmp);
        new_node->next = NULL;
        if (!parse_list)
        {
            parse_list = new_node;
            current = parse_list;
        }
        else
        {
            current->next = new_node;
            current = current->next;
        }
        // advance tmp past current command and its pipe
        while (tmp && strncmp(tmp->token, "|", 2) != 0)
            tmp = tmp->next;
        if (tmp && strncmp(tmp->token, "|", 2) == 0)
            tmp = tmp->next; // skip the pipe itself
    }
    return (parse_list);
}

/* extracts char*** from t_parse list to feed into work() */
static char ***parse_to_cmds(t_parse *list, int len)
{
    char    ***cmds;
    int     i;

    cmds = malloc(sizeof(char **) * (len + 1));
    if (!cmds)
        return (NULL);
    i = 0;
    while (list && i < len)
    {
        cmds[i] = list->cmd;
        list = list->next;
        i++;
    }
    cmds[i] = NULL;
    return (cmds);
}

/* the main entry point — ties everything together */
int are_we_gonna_split(t_lexer *head, t_env **env)
{
    t_parse     *parse_list;
    char        ***cmds;
    int         len;

    if (!head)
        return (1);
    len = count_commands(head);
    if (len == 1)
    {
        // no pipes — run directly
        char **cmd = organize(head);
        apply_redirections(head);
        return (execute_command(cmd, env), 0);
    }
    parse_list = split_into_processes(head);
    if (!parse_list)
        return (1);
    cmds = parse_to_cmds(parse_list, len);
    if (!cmds)
        return (1);
    return (work(len, cmds, env));
}
// ```

// ---

// **How it all connects now:**
// ```
// are_we_gonna_split(head, env)
//     │
//     ├── 1 command?  ──► organize() ──► apply_redirections() ──► execute_command()
//     │
//     └── multiple?
//             │
//             ▼
//       split_into_processes()     ← splits t_lexer on | into t_parse list
//             │
//             ▼
//       parse_to_cmds()            ← extracts char*** from t_parse
//             │
//             ▼
//       work(len, cmds, env)       ← your pipe/fork engine from before
//             │
//             ▼
//       create_processes()         ← forks children, wires pipes
//             │
//             ▼
//       apply_redirections()  +  execute_command()   ← runs in each child