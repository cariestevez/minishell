#include "../minishell.h"

int	unset(t_shell *shell, t_simple_cmds *cmd)
{
	int	i;
	char	*var;

	var = cmd->str[2];
	i = 0;
	while (ft_strncmp(var, shell->env[i], ft_strlen(var)) != 0)
		i++;
	free(shell->env[i]);
	while (shell->env[i] != NULL)
	{
		shell->env[i] = shell->env[i + 1];
		i++;
	}
	return (0);
}