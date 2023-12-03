#include "minishell.h"

void    export_print(t_shell *shell)
{
    int i;

    i = 0;
	while(shell->env[i] != NULL)
	{
        ft_putstr_fd("declare -x ", 1);
		ft_putstr_fd(shell->env[i], 1);
        write(1, "\n", 1);
		i++;
	}
}

int ft_export(t_shell *shell, t_simple_cmds *cmd)
{
    int i;

    i = 1;
    if (cmd->str[i] == NULL) //if no args are given, prints the local variables
	{
	    export_print(shell);
	    return (0);
    }
    while(cmd->str[i])
    {
        if (declare_variable(cmd->str[i], shell->env) != 0)
            return (-1);
        i++;
    }
    return (0);
}