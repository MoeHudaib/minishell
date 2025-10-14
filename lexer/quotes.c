#include "quotes.h"

int has_quote(char *str)
{
    int i;

    i = 0;
    while (str[i])
    {
        if (str[i] == '\'' || str[i] == '"')
            return (1);
        i++;
    }
    return (0);
}

int	has_unclosed_quotes(const char *s)
{
	int		i = 0;
	char	quote = 0;

	while (s[i])
	{
		if (s[i] == '\\' && quote != '\'')
		{
			if (s[i + 1])
				i++;
		}
		else if (s[i] == '\'' || s[i] == '"')
		{
			if (!quote)
				quote = s[i];
			else if (quote == s[i])
				quote = 0;
		}
		i++;
	}
	return (quote != 0);
}

char	*read_full_input(char *str)
{
	char *line;
	char *temp;
    char *cwd = ft_strjoin(str, " -> ");
	char *full = readline(cwd);
    free(cwd);
	while (full && has_unclosed_quotes(full))
	{
		temp = readline("> ");
		if (!temp)
			break;
		char *joined = malloc(strlen(full) + strlen(temp) + 2);
		sprintf(joined, "%s\n%s", full, temp);
		free(full);
		free(temp);
		full = joined;
	}
	return (full);
}

char **split_with_quotes(const char *str)
{
    char **result = malloc(256 * sizeof(char *));
    int count = 0, i = 0;

    while (str[i])
    {
        while (str[i] == ' ')
            i++;
        if (!str[i])
            break;
        char buffer[1024];
        int b = 0;
        char quote = 0;
        while (str[i])
        {
            if (!quote && (str[i] == '"' || str[i] == '\''))
            {
                quote = str[i++];
                continue;
            }
            if (quote == '"')
            {
                if (str[i] == '\\' && str[i + 1])
                {
                    if (str[i + 1] == '"' || str[i + 1] == '\\' ||
                        str[i + 1] == '$' || str[i + 1] == '`')
                    {
                        buffer[b++] = str[i + 1];
                        i += 2;
                        continue;
                    }
                }
                else if (str[i] == '"')
                {
                    quote = 0;
                    i++;
                    continue;
                }
            }
            else if (quote == '\'')
            {
                if (str[i] == '\'')
                {
                    quote = 0;
                    i++;
                    continue;
                }
            }
            else if (str[i] == ' ')
                break;
            else if (str[i] == '\\' && str[i + 1])
            {
                buffer[b++] = str[i + 1];
                i += 2;
                continue;
            }
            buffer[b++] = str[i++];
        }
        buffer[b] = '\0';
        result[count++] = strdup(buffer);
    }
    result[count] = NULL;
    return result;
}


// int main(void)
// {
// 	char *line;

//     char cwd[1024];
//     getcwd(cwd, 1024);
// 	while ((line = read_full_input(cwd)))
// 	{
// 		if (!*line)
// 		{
// 			free(line);
// 			continue;
// 		}
// 		add_history(line);
//         ft_strtrim(line, "\" '");
//         int flag = has_quote(line);
// 		printf("Final input: [%s]\n%d\n", line, flag);
//         char **str = split_with_quotes(line);
//         printf("%s\n%s\n", str[0], str[1]);
// 		free(line);
// 	}
// 	printf("exit\n");
// 	return (0);
// }