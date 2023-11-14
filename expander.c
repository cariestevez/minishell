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

char    *expand_var(char *str, char **env)
{
    //not working, correct version not pushed from esthers laptop
    //edgecases not handled
    int     i;
    int     start;
    char    *var;
    char    *ret;
    char    *append;

    i = 0;
    start = 0;
    while (str[i])
    {
        if (str[i] == '$')           
            start = i + 1;
        i++;
        if (str[start - 1] == '$' && ft_isalpha(str[i]) != 1)
            break ;
    }
    //get the variable from env
    var = ft_getenv(ft_substr(str, start, i - start), env);
     if (!var)
        return (NULL);
    //malloc for the epanded string
    ret = (char *)malloc(sizeof(char) * (ft_strlen(str) - (i - start) + ft_strlen(var)));
    //copy the initial part of the str without the variable
    ft_strlcpy(ret, str, start);
    //concatinate the variable onto the initial part of the str
    ft_strlcat(ret, var, ft_strlen(var) + start + 1);
    //get the remaining part of the str
    append = ft_substr(str, i, ft_strlen(str) - i);
    //concatinate remaining part of str onto the expanded str
    ft_strlcat(ret, append, ft_strlen(append) + ft_strlen(ret) + 1);
    free(append);
    //free(str);
    if (str[i] != '\0')
        expand_var(ret, env);
    return (ret);
}

/*char    *expand_arg(char *str, int start)
{

}*/

char    *expand_exit(char *str, int start)
{
    char *exit;
    //char    *append;
    char    *ret;

    (void)start;
    exit = "127";
    ret = exit;
    free(str);
    return (ret);
}

int expander(t_simple_cmds *cmds)
{
    int            i;
    int             j;

    i = 0;
    j = 0;
    while (cmds)
    {
         while (cmds->str[i])
         {
            //when single quotes, envvars are not expanded
               /*  if (cmds->str[i] == 39)
                {
                    i++;
                    while (cmds->str[i] != 39)
                        j++;
                }
            //$? prints the exit code   
                if (cmds->str[i] == '$')
                {
                    if (cmds->str[i + 1] == '?')
                        cmds->str = expand_exit(cmds->str[i], j);
                    else if (isdigit(cmds->str[i][j + 1]))
                        cmds->str[i] = expand_arg(cmds->str[i], j);
                    else
                        cmds->str[i] = expand_var(cmds->str, i + 1, cmds->env);
                } */
                i++;
            }
        cmds = cmds->next;
    }
    return (0);
}
