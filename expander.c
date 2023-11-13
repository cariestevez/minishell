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
    int     i;
		input		start;
    char    *var;
    char    *ret;
    char    *append;

    i = start;
    while (str[i])
    {
			if(str[i] == '$')
				start = i + 1;
      if (!str[i] || ft_isalpha(str[i]) != 1)
            break ;
        i++;
    }
    //find the variable name in env
    var = ft_getenv(ft_substr(str, start, i - start), env);
     if (!var)
        return (NULL);
    ret = (char *)malloc(sizeof(char) * (ft_strlen(str) - (i - start) + ft_strlen(var)));
    ft_strlcpy(ret, str, start);
    ft_strlcat(ret, var, ft_strlen(var) + start + 1);
    append = ft_substr(str, i, ft_strlen(str) - i);
    ft_strlcat(ret, append, ft_strlen(append) + ft_strlen(ret) + 1);
    //free(str);
    free(append);
		if (str[i] != NULL)
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
  //  int             j;

    i = 0;
    //j = 0;
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
