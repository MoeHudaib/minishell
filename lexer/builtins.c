#include "quotes.h"
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>

// ---------------------------------------------------------------------
// Utility helpers

static void update_pwd_vars(char ***envp)
{
    char cwd[1024];
    if (getcwd(cwd, sizeof(cwd)))
    {
        setenv("PWD", cwd, 1);
    }
}

// ---------------------------------------------------------------------
// Builtins

int is_builtin(const char *cmd)
{
    return (strcmp(cmd, "cd") == 0 ||
            strcmp(cmd, "exit") == 0 ||
            strcmp(cmd, "export") == 0 ||
            strcmp(cmd, "unset") == 0 ||
            strcmp(cmd, "pwd") == 0);
}

int run_builtin(t_command *cmd, char ***envp)
{
    // === cd ===
    if (strcmp(cmd->args[0], "cd") == 0)
    {
        const char *target = cmd->args[1] ? cmd->args[1] : getenv("HOME");
        if (!target)
        {
            fprintf(stderr, "cd: HOME not set\n");
            return 1;
        }
        char oldpwd[1024];
        getcwd(oldpwd, sizeof(oldpwd));
        if (chdir(target) != 0)
        {
            perror("cd");
            return 1;
        }
        setenv("OLDPWD", oldpwd, 1);
        update_pwd_vars(envp);
        return 0;
    }

    // === pwd ===
    if (strcmp(cmd->args[0], "pwd") == 0)
    {
        char cwd[1024];
        if (getcwd(cwd, sizeof(cwd)))
            printf("%s\n", cwd);
        else
            perror("pwd");
        return 0;
    }

    // === exit ===
    if (strcmp(cmd->args[0], "exit") == 0)
    {
        int code = 0;
        if (cmd->args[1])
            code = atoi(cmd->args[1]);
        printf("exit\n");
        exit(code);
    }

    // === export ===
    if (strcmp(cmd->args[0], "export") == 0)
    {
        if (!cmd->args[1])
        {
            extern char **environ;
            for (int i = 0; environ[i]; i++)
                printf("declare -x %s\n", environ[i]);
            return 0;
        }
        for (int i = 1; cmd->args[i]; i++)
        {
            char *eq = strchr(cmd->args[i], '=');
            if (!eq)
            {
                // Variable with no value (like `export VAR`)
                setenv(cmd->args[i], "", 1);
            }
            else
            {
                *eq = '\0';
                setenv(cmd->args[i], eq + 1, 1);
            }
        }
        return 0;
    }

    // === unset ===
    if (strcmp(cmd->args[0], "unset") == 0)
    {
        for (int i = 1; cmd->args[i]; i++)
            unsetenv(cmd->args[i]);
        return 0;
    }

    return -1;
}

char *expand_variables(char *input, char **env)
{
    char *result = malloc(strlen(input) * 4 + 1); // rough allocation
    int ri = 0, i = 0;

    while (input[i])
    {
        if (input[i] == '$' && input[i+1] && (isalnum(input[i+1]) || input[i+1] == '_'))
        {
            i++;
            char var[128];
            int vi = 0;
            while (isalnum(input[i]) || input[i] == '_')
                var[vi++] = input[i++];
            var[vi] = '\0';

            char *val = getenv(var);
            if (val)
            {
                strcpy(result + ri, val);
                ri += strlen(val);
            }
        }
        else
            result[ri++] = input[i++];
    }
    result[ri] = '\0';
    return result;
}
