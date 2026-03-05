#ifndef ENV_H
# define ENV_H

#include <unistd.h>
#include <stdlib.h>
#include "../libs/libft/libft.h"

typedef struct s_env
{
    char *key;
    char *value;
    struct s_env *next;
} t_env;


void	ft_unset(t_env **head, char *key);
t_env    *set_env(char **env);
t_env	*seperate_key_value(char *line, t_env *new_one);
t_env	*add_last(t_env **head, t_env *node);
int	print_list(t_env *head);
void	envclear(t_env **head);
int    ft_export(t_env **head, char  *line);

#endif