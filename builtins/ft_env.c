#iclude "minishell.h"

int	ft_env(t_shell *shell)
{
	while(shell->env[i] != NULL)
	{
		ft_putstr_fd(env[i], 1);
		i++;
	}
	return (0);
}