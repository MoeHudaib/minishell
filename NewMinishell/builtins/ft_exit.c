#include "../organize.h"

int	ft_exit(char **cmd, int last_status)
{
	int	status;

	write(STDOUT_FILENO, "exit\n", 5);
	if (!cmd[1])
		exit(last_status);         // no arg → exit with last $?
	if (cmd[2])                    // too many args → error, don't exit
	{
		ft_putendl_fd("minishell: exit: too many arguments", STDERR_FILENO);
		return (1);
	}
	if (!ft_isnumeric(cmd[1]))     // non-numeric arg → error + exit 2
	{
		ft_putstr_fd("minishell: exit: ", STDERR_FILENO);
		ft_putstr_fd(cmd[1], STDERR_FILENO);
		ft_putendl_fd(": numeric argument required", STDERR_FILENO);
		exit(2);
	}
	status = ((ft_atoi(cmd[1]) % 256) + 256) % 256;
	exit(status);
}
