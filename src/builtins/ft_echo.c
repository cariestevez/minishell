#include "minishell.h"

static int	print_args(char **display, t_shell *shell, int i)
{
	while (display[i] != NULL)
	{
		ft_putstr_fd("echo wants to write\n", 1);
		display[i] = variable_expansion(display[i], shell);
		if (display[i] == NULL)
			return (-1);
		ft_putstr_fd(display[i], 1);
		i++;
		if (display[i] != NULL)
			ft_putchar_fd(' ', 1);
	}
	return (0);
}

int	ft_echo(t_shell *shell, t_simple_cmds *cmd)
{
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
	print_args(cmd->str, shell, flag_check);
	if (flag_check == 1)
		write(1, "\n", 1);
	return (0);
}