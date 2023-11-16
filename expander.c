#include "minishell.h"
/*missing:
- expand exit code $?
- expand arguments $1/2/3...
- find more edge cases*/

char *replace_variable(char *str, int start, int end, char **env)
{ 
    //something weird not working here!!!
    char    *var;
    char    *ret;
    char    *append;
    //trims the variable and gets the env value
    var = ft_substr(str, start, end - start);
    var = ft_strtrim(var, "}${");
    var = ft_getenv(var, env);
    if (!var)
        return (NULL);
    //joins first part of str with the expanded var
    ret = ft_substr(str, 0, start);
    ret = ft_strjoin(ret, var);
    //get the last part of the str and join with ret str
    append = ft_substr(str, end, ft_strlen(str) - end + 1);
    ret = ft_strjoin(ret, append);
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

int		declare_variable(char *var, char **locvars)
{
	//the syntax of var should be name=[value]
    //adds the new variable to the end of the array
	int	i;

	i = 0;
    if (!locvars)
    {
        locvars = ft_calloc(sizeof(char *), 1);
        if (!locvars)
            return (-1);
    }
    while (locvars[i] != NULL)
		i++;
	locvars[i] = ft_strdup(var);
    if (!locvars[i])
    {
        return (DECLARE_VAR_ERROR);
    }
    locvars[i + 1] = NULL;
	return (0);
}

int expander(t_shell *shell)
{
    int            i;

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
    return (SUCCESS);
}