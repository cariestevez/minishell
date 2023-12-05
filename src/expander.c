#include "minishell.h"
/*missing:
- expand exit code $?
- find more edge cases*/// -> single quotes

char *replace_variable(char *str, int start, int end, t_shell *shell)
{ 
    char    *var;
    char    *tmp;
    char    *ret;
    char    *append;
    //trims the variable and gets the env value
    
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
    //joins first part of str with the expanded var
    ret = ft_substr(str, 0, start);
    tmp = ft_strjoin(ret, var);
    free(ret);
    //get the last part of the str and join with ret str
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
            str = replace_variable(str, start, i, shell);
            if (!str)
                return (NULL);
        }
        if (str[i] == '$' && 
            (ft_isalnum(str[i + 1]) == 1 || ft_strchr("_{}", str[i + 1]) != 0))
        {
            start = i;
            i++;
            while (ft_isalnum(str[i]) == 1 || ft_strchr("_{}", str[i]) != 0)
                i++;
            str = replace_variable(str, start, i, shell);
            if (!str)
                return (NULL);
        }
        i++;
    } 
    return (str);
}

int		declare_variable(char *var, t_shell *shell)
{
	//the syntax of var should be name=[value]
    //adds the new variable to the end of the array
	int	i;

	i = 0;
    if (!shell->env)
        return (-1);
    if (ft_strchr(var, '=') == 0) //wrong syntax
        return (1);
    variable_expansion(var, shell);
    while (shell->env[i] != NULL)
    {
        if (ft_strncmp(shell->env[i], var, ft_strlen(var)) == 0)
            return (1);
		i++;
    }
    free(shell->env[i]);
	shell->env[i] = ft_strdup(var);
    if (!shell->env[i])
    {
        return (DECLARE_VAR_ERROR);
    }
    shell->env[i + 1] = NULL;
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
         while (shell->cmds->str[i])
         {
            shell->cmds->str[i] = variable_expansion(shell->cmds->str[i], shell);
            if (shell->cmds->str[i] == NULL)
            {
                tmp = shell->cmds->str[i];
                shell->cmds->str[i] = shell->cmds->str[i + 1];
                free(tmp);
            }
            i++;
        }
        shell->cmds = shell->cmds->next;
    }
    //quote removal
    shell->cmds = head;
    return (SUCCESS);
}
