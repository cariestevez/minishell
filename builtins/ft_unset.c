#include "minishell.h"

int	unset(t_shell *shell)
{
	int	i;
	char	*var;

	var = shell->cmds->str[2];
	i = 0;
	while (ft_strncmp(var), env[i], ft_strlen(var) != 0)
		i++;
	free(env[i]);
	while (env[i] != NULL)
	{
		env[i] == env[i + 1];
		i++;
	}
	return (0);
}