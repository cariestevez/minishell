#include "minishell.h"
/*missing:
- expand exit code $?
- expand arguments $1/2/3...
- find more edge cases*/// -> single quotes

char *replace_variable(char *str, int start, int end, char **env)
{ 
    char    *var;
    char    *tmp;
    char    *ret;
    char    *append;
    //trims the variable and gets the env value
    var = ft_substr(str, start, end - start);
    tmp = ft_strtrim(var, "}${"); 
    free(var);
    var = ft_getenv(tmp, env);
    if (!var)
        return (NULL);
    free(tmp);
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

char    *variable_expansion(char *str, char **env)
{
    int     i;
    int     start;
   
    i = 0;
    start = 0;
    while (str[i] != '\0')
    {
        if (str[i] == '$' && 
            (ft_isalnum(str[i + 1]) == 1 || ft_strchr("_{}", str[i + 1]) != 0))
        {
            start = i;
            i++;
            while (ft_isalnum(str[i]) == 1 || ft_strchr("_{}", str[i]) != 0)
                i++;
            str = replace_variable(str, start, i, env);
            if (!str)
                return (NULL);
        }
        i++;
    } 
    return (str);
}
//the syntax of var should be name=[value]
//adds the new variable to the end of the array
int		declare_variable(char *var, char **env)
{
	int	i;

	i = 0;
    if (!env)
        return (-1);
    if (ft_strchr(var, '=') == 0)
        return (1);
    variable_expansion(var, env);
    while (env[i] != NULL)
    {
        if (ft_strncmp(env[i], var, ft_strlen(var)) == 0)
            return (1);
		i++;
    }
    free(env[i]);
	env[i] = ft_strdup(var);
    if (!env[i])
    {
        return (DECLARE_VAR_ERROR);
    }
    env[i + 1] = NULL;
	return (0);
}

int expander(t_shell *shell)
{
    t_simple_cmds *head;
    int            i;

    head = shell->cmds;
    i = 0;
    while (shell->cmds)
    {
         while (shell->cmds->str[i])
         {
            shell->cmds->str[i] = variable_expansion(shell->cmds->str[i], shell->env);
            if (shell->cmds->str == NULL)
                return (EXPANDER_VAR_ERROR);
             i++;
        }
        shell->cmds = shell->cmds->next;
    }
    //quote removal
    shell->cmds = head;
    return (SUCCESS);
}
