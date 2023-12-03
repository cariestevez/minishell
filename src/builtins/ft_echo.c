#include "minishell.h"

int	ft_echo(t_shell *shell, t_simple_cmds *cmd)
{
	char	*display;
	int		flag_check;

	if (!cmd->str[1])
	{
		write(1, "\n", 1);
		return (0);
	}
	if (ft_strncmp(cmd->str[1], "-n", BUFFER) == 0)
	{
		if (!cmd->str[2])
			return (0);
		flag_check = 2;
	}
	else
		flag_check = 1;
	display = cmd->str[flag_check];
	variable_expansion(display, shell->env);
	if (display == NULL)
		return (-1);
	ft_putstr_fd(display, 1);
	if (flag_check == 1)
		write(1, "\n", 1);
	return (0);
}