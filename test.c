#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include "./parse/parse.h"
#include <sys/wait.h>
#include <signal.h>

void handle_sigint(int sig)
{
    (void)sig;
    write(1, "\nCaught SIGINT (Ctrl+C)\n", 25);
}

int has_quote(char *str, char c)
{
    int i;

    i = 0;
    while (str[i])
    {
        if (str[i] == c)
            return (1);
        i++;
    }
    return (0);
}

int handle_quote(char *str, char c)
{
    int i;
    int counter;

    counter = 0;
    i = 0;
    while (str[i])
    {
        if (str[i] == c)
            counter++;
        i++;
    }
    if (counter % 2 == 0)
        return (1);
    return (0);
}

#include <stdlib.h>
#include <string.h>

static size_t	count_sub(const char *str, const char *sub)
{
	size_t	count = 0;
	size_t	sublen = strlen(sub);

	if (!*sub)
		return (0);
	while ((str = strstr(str, sub)))
	{
		count++;
		str += sublen;
	}
	return (count);
}

char	*ft_strreplace(const char *str, const char *sub, const char *replace)
{
	size_t	sub_len = strlen(sub);
	size_t	rep_len = strlen(replace);
	size_t	count = count_sub(str, sub);
	size_t	new_len = strlen(str) + count * (rep_len - sub_len);
	char	*result = malloc(new_len + 1);
	char	*p = result;
	const char *pos;

	if (!result)
		return (NULL);
	while ((pos = strstr(str, sub)))
	{
		size_t	before = pos - str;
		memcpy(p, str, before);
		p += before;
		memcpy(p, replace, rep_len);
		p += rep_len;
		str = pos + sub_len;
	}
	strcpy(p, str);
	return (result);
}
int main(int ac, char **av, char **env)
{
    char pwd[1024];
    char line[1024];
    
    signal(SIGINT, handle_sigint);
    while (1)
    { 
        getcwd(pwd, sizeof(pwd));
        printf("%s -> ", pwd);
        fflush(stdout);
        if (!fgets(line, sizeof(line), stdin))
            break;
        line[strcspn(line, "\n")] = 0;
        if (strlen(line) == 0)
            continue;
        char **cmd = ft_split(line, ' ');
        if (!cmd || !cmd[0])
            continue;
        int id = fork();
        if (id == 0)
        { 
            char *path = build_path(env, cmd[0]);
            if (path)
                execve(path, cmd, env);
            perror("command not found");
            exit(1);
        }
        else if (id > 0)
            wait(NULL);
    }
    return 0;
}