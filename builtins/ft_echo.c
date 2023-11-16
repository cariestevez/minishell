#include "../minishell.h"

int	ft_echo(t_shell *shell, t_simple_cmds *cmd)
{
	char	*display;

	display = cmd->str[1];
	variable_expansion(display, shell->env);
	if (display < 0)
		return (-1);
	ft_putstr_fd(display, 1);
	if (ft_strncmp(cmd->str[2], "-n", 3) != 0)
		write(1, "\n", 1);
	return (0);
}