#include "../organize.h"

int	ft_isnumeric(char *str)
{
	int	i;

	i = 0;
	if (str[i] == '-' || str[i] == '+')
		i++;
	if (!str[i])
		return (0);
	while (str[i])
	{
		if (str[i] < '0' || str[i] > '9')
			return (0);
		i++;
	}
	return (1);
}

int	is_builtin(char *cmd)
{
	if (!cmd)
		return 0;

	if (!ft_strcmp(cmd, "cd"))
		return 1;
	if (!ft_strcmp(cmd, "echo"))
		return 1;
	if (!ft_strcmp(cmd, "pwd"))
		return 1;
	if (!ft_strcmp(cmd, "export"))
		return 1;
	if (!ft_strcmp(cmd, "unset"))
		return 1;
	if (!ft_strcmp(cmd, "env"))
		return 1;
	if (!ft_strcmp(cmd, "exit"))
		return 1;
	return 0;
}

int	exec_builtin(char **cmd, t_env **env, int last_status)
{
	if (!cmd || !cmd[0])
		return (0);
	if (!ft_strcmp(cmd[0], "echo"))
		return (ft_echo(cmd));
	if (!ft_strcmp(cmd[0], "pwd"))
		return (ft_pwd());
	if (!ft_strcmp(cmd[0], "cd"))
		return (ft_cd(cmd, env));
	if (!ft_strcmp(cmd[0], "env"))
		return (ft_env(*env));
	if (!ft_strcmp(cmd[0], "export"))
		return (ft_export(env, cmd));
	if (!ft_strcmp(cmd[0], "unset"))
		return (ft_unset(env, cmd));
	if (!ft_strcmp(cmd[0], "exit"))
		return (ft_exit(cmd, last_status));
	return (1);
}
