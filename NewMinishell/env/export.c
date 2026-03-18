#include "../organize.h"

static int	export_one(t_env **head, char *arg)
{
	t_env	*new_one;

	new_one = seperate_key_value(arg, NULL);
	if (!new_one)
	{
		ft_putstr_fd("minishell: export: `", STDERR_FILENO);
		ft_putstr_fd(arg, STDERR_FILENO);
		ft_putendl_fd("': not a valid identifier", STDERR_FILENO);
		return (1);
	}
	add_last(head, new_one);
	return (0);
}

int	ft_export(t_env **head, char **cmd)
{
	int	i;
	int	status;

	if (!cmd[1])
	{
		ft_env(*head);
		return (0);
	}
	i = 1;
	status = 0;
	while (cmd[i])
	{
		if (export_one(head, cmd[i]))
			status = 1;
		i++;
	}
	return (status);
}

// int main(int ac, char **av, char **env)
// {
//     t_env   *head;
//     head = set_env(env);
//     ft_export(&head, "MOHAMMAD=SiwarHussein");
//     print_list(head);
//     envclear(&head);
// }