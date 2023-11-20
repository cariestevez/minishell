#include "minishell.h"

int	ft_env(t_shell *shell, t_simple_cmds *cmd)
{
	int	i;

	(void)cmd;
	i = 0;
	while(shell->env[i] != NULL)
	{
		ft_putstr_fd(shell->env[i], 1);
		i++;
	}
	return (0);
}