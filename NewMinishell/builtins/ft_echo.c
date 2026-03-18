#include "../organize.h"

int	ft_echo(char **cmd)
{
	int	i;
	int	newline;

	i = 1;
	newline = 1;
	if (cmd[1] && ft_strcmp(cmd[1], "-n") == 0)
	{
		newline = 0;
		i++;
	}
	while (cmd[i])
	{
		printf("%s", cmd[i]);
		if (cmd[i + 1])
			printf(" ");
		i++;
	}
	if (newline)
		printf("\n");
	return (0);
}
