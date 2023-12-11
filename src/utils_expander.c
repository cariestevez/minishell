#include "minishell.h"

int quote_removal(t_shell *shell, int i)
{
    if (shell->cmds->str[i][0] == '\"')
        shell->cmds->str[i] = ft_strtrim(shell->cmds->str[i], "\"");
    else if (shell->cmds->str[i][0] == '\'')
        shell->cmds->str[i] = ft_strtrim(shell->cmds->str[i], "\'");
    return (0);
}

static int replace_variable(char *var, char *new_var, t_shell *shell, int i)
{
    char    **current_env;

    current_env = ft_split(shell->env[i], '=');
    if (ft_strncmp(current_env[0], new_var, ft_strlen(new_var)) == 0)
    {
        free(current_env);
        free(new_var);
        free(shell->env[i]);
	    shell->env[i] = ft_strdup(var);
        return (0);
    }
    free(current_env);
    return (1);
}

static int add_variable(char *var, t_shell *shell, int i)
{
    char    **new_env;

    new_env = ft_calloc(sizeof(char *), i + 2);
    if (!new_env)
        return (-1);
    new_env[i + 1] = NULL;
    new_env[i] = ft_strdup(var);
    while (--i >= 0)
        new_env[i] = ft_strdup(shell->env[i]);
    free(shell->env);
    shell->env = new_env;
    return (0);
}

int		declare_variable(char *var, t_shell *shell)
{
	int	                  i;
    char         **new_var;

	i = 0;
    if (ft_strrchr(var, '=') == 0)
        return (-1);
    check_for_variables(var, shell);
    new_var = ft_split(var, '=');
    while (shell->env[i] != NULL)
    {
        if (ft_strncmp(shell->env[i], var, ft_strlen(var)) == 0)
            return (free(new_var), 1);
        if (replace_variable(var, new_var[0], shell, i) == 0)
            return (free(new_var), 0);
		i++;
    }
    free(new_var);
    add_variable(var, shell, i);
    return (0);
}