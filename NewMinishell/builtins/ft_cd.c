#include "../organize.h"

static int	get_target(char **cmd, t_env **env, char **target)
{
	if (!cmd[1])
	{
		*target = get_env_value("HOME", env_to_array(*env));
		if (!*target)
		{
			ft_putendl_fd("minishell: cd: HOME not set", STDERR_FILENO);
			return (1);
		}
	}
	else
		*target = cmd[1];
	return (0);
}

int	ft_cd(char **cmd, t_env **env)
{
	char	*oldpwd;
	char	*target;
	char	buf[4096];

	oldpwd = get_env_value("PWD", env_to_array(*env));
	if (get_target(cmd, env, &target))
		return (1);
	if (chdir(target) != 0)
	{
		ft_putstr_fd("minishell: cd: ", STDERR_FILENO);
		ft_putstr_fd(target, STDERR_FILENO);
		ft_putendl_fd(": No such file or directory", STDERR_FILENO);
		return (1);
	}
	if (!getcwd(buf, sizeof(buf)))
		return (1);
	if (oldpwd)
		update_env("OLDPWD", oldpwd, env);
	update_env("PWD", buf, env);
	return (0);
}
