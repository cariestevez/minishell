#include "minishell.h"
/*missing:
- expand exit code $?
- expand arguments $1/2/3...
- find more edge cases*/

char	*ft_getenv(char *name, char **env)
{
	int		i;
	int		j;
	char	*sub;

	i = 0;
	while (env[i])
	{
		j = 0;
		while (env[i][j] && env[i][j] != '=')
			j++;
		sub = ft_substr(env[i], 0, j);
		if (ft_strncmp(sub, name, 100) == 0)
		{
			free(sub);
			return (env[i] + j + 1);
		}
		free(sub);
		i++;
	}
	return (NULL);
}

char *replace_variable(char *str, int start, int end, char **env)
{ 
    char    *var;
    char    *ret;
    char    *append;

    var = ft_substr(str, start, end - start);
    //trims the variable and gets the env value
    var = ft_getenv(ft_strtrim(var, "${}"), env);
    if (!var)
        return (NULL);
    //joins first part of str with the expanded var
    ret = ft_strjoin(ft_substr(str, 0, start), var);
    //get the last part of the str and join with ret str
    append = ft_substr(str, end, ft_strlen(str) - end);
    ret = ft_strjoin(ret, append);
    free(append);
    return (ret);
}

int    param_expansion(char *str, char **env)
{
    int     i;
    char     *start;
    int     end;
   
    i = 0;
    start = 0;
    while (str[i] != '\0')
    {
        if (str[i] == '$')
        {
            start = i;
            while (ft_isalnum(str[i]) == 1 || ft_strncmp("_{}", str[i], 3) > 0)
                i++;
            str = replace_variable(str, start, i, env);
            if (!str)
                return (-1);
        }
        i++;
    }
    return (0);
}

int		declare_variable(char *var, char **locvars)
{
	//the syntax of var should be name=[value]
    //adds the new variable to the end of the array
	int	i;

	i = 0;
    if (!locvars)
        locvars = ft_calloc(sizeof(char *), 1);
    if (!locvars)
        return (-1);
	while (locvars[i] != NULL)
		i++;
	locvars[i] = ft_strdup(var);
    if (!locvars[i])
        return (-1);
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
           //brace expansion
           //tilde expansion 
            if (param_expansion(shell->cmds->str[i], shell->env) != 0)
                return(exitcode);
           //command substitution 
           //arithmetic expansion 
           //word splitting 
           //filename expansion
           i++;
        }
        shell->cmds = shell->cmds->next;
    }
    //quote removal
    return (0);
}
