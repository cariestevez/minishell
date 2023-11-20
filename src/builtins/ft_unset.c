#include "minishell.h"

int	remove_var(char **list, t_simple_cmds *cmd)
{
	int	i;
	int	j;

	i = 1;
	j = 0;
	while (cmd->str[i])
	{
			j = 0;
			while (ft_strncmp(cmd->str[i], list[j], ft_strlen(cmd->str[i])) != 0
				&& list[j] != NULL)
			{
				j++;
			}
			if (cmd->str[i] != NULL)
			{
				free(list[j]);
				while (list[j] != NULL)
				{
					list[j] = list[j + 1];
					j++;
				}
				free(list[j]);
			}
			i++;
	}
	return (0);
}

int	unset(t_shell *shell, t_simple_cmds *cmd)
{
	int	i;
	int	j;

	i = 1;
	j = 0;

	remove_var(shell->env, cmd);
	return (0);
}