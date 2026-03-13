#include "../organize.h"

int    ft_export(t_env **head, char  *line)
{
    t_env   *new_one;

    if (!line || !head)
        return (1);
    new_one = seperate_key_value(line, NULL);
    add_last(head, new_one);
    return (0);
}

// int main(int ac, char **av, char **env)
// {
//     t_env   *head;
//     head = set_env(env);
//     ft_export(&head, "MOHAMMAD=SiwarHussein");
//     print_list(head);
//     envclear(&head);
// }