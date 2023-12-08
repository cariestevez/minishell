#include "minishell.h"

char *get_expanded_variable(char *str, int start, int end, t_shell *shell)
{ 
    char    *var;
    char    *tmp;
    char    *ret;
    char    *append;
    
    if (str[start] == '$' && str[start + 1] == '?')
        var = ft_itoa(shell->exitcode);
    else
    {
        var = ft_substr(str, start, end - start);
        tmp = ft_strtrim(var, "}${"); 
        free(var);
        var = ft_getenv(tmp, shell->env);
        if (!var)
            var = "";
         free(tmp);
    }
    ret = ft_substr(str, 0, start);
    tmp = ft_strjoin(ret, var);
    free(ret);
    append = ft_substr(str, end, ft_strlen(str) - end + 1);
    ret = ft_strjoin(tmp, append);
    free(tmp);
    free(append);
    return (ret);
}

char    *variable_expansion(char *str, t_shell *shell)
{
    int     i;
    int     start;
   
    i = 0;
    start = 0;
    while (str[i] != '\0')
    {
        if  (str[i] == '$' && str[i + 1] == '?')
        {
            start = i;
            i += 2;
            str = get_expanded_variable(str, start, i, shell);
            if (!str)
                return (NULL);
        }
        if (str[i] == '$' && str[i + 1] != '\0' && (ft_isalnum(str[i + 1]) == 1 || ft_strchr("_{}", str[i + 1]) != 0))
        {
            start = i;
            i++;
            while (ft_isalnum(str[i]) == 1 || ft_strchr("_{}", str[i]) != 0)
                i++;
            str = get_expanded_variable(str, start, i, shell);
            if (!str)
                return (NULL);
        }
        i++;
    }
    return (str);
}

static int replace_variable(char *var, char *new_var, t_shell *shell, int i)
{
    char    **current_env;

    current_env = ft_split(shell->env[i], '=');
   // ft_printf("checking %s\n", current_env[0]);
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
    variable_expansion(var, shell);
    new_var = ft_split(var, '=');
    while (shell->env[i] != NULL)
    {
        if (ft_strncmp(shell->env[i], var, ft_strlen(var)) == 0)
            return (1);
        if (replace_variable(var, new_var[0], shell, i) == 0)
            return (0);
		i++;
    }
    free(new_var);
    add_variable(var, shell, i);
    return (0);
}

int expander(t_shell *shell)
{
    t_simple_cmds *head;
    char        *tmp;
    int            i;

    head = shell->cmds;
    i = 0;
    while (shell->cmds)
    {
        i = 0;
         while (shell->cmds->str[i] != NULL)
         {       
            shell->cmds->str[i] = variable_expansion(shell->cmds->str[i], shell);
            if (shell->cmds->str[i] == NULL)
            {
                tmp = shell->cmds->str[i];
                shell->cmds->str[i] = shell->cmds->str[i + 1];
                free(tmp);
            }
            if (shell->cmds->str[i][0] == '\"')
                shell->cmds->str[i] = ft_strtrim(shell->cmds->str[i], "\"");
            else if (shell->cmds->str[i][0] == '\'')
                shell->cmds->str[i] = ft_strtrim(shell->cmds->str[i], "\'");
            ft_printf("str is %s\n", shell->cmds->str[i]);
            i++;
        }
        shell->cmds = shell->cmds->next;
    }
    shell->cmds = head;
    return (0);
}
