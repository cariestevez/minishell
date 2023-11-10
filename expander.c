#include "minishell.h"
/*missing:
- expand exit code $?
- expand arguments $1/2/3...
- find more edge cases*/
char    *expand_var(char *str, int start)
{
    int i;
    char    *var;
    char    *ret;
    char    *append;

    i = start;
    while (str[i])
    {
        if (!str[i] || ft_isalpha(str[i]) != 1)
            break ;
        i++;
    }
    var = getenv(ft_substr(str, start, i - start ));
     if (!var)
        return (NULL);
    ret = (char *)malloc(sizeof(char) * (ft_strlen(str) - (i - start) + ft_strlen(var)));
    ft_strlcpy(ret, str, start);
    ft_strlcat(ret, var, ft_strlen(var) + start + 1);
    append = ft_substr(str, i, ft_strlen(str) - i);
    ft_strlcat(ret, append, ft_strlen(append) + ft_strlen(ret) + 1);
    free(str);
    free(append);
    ft_printf("expanded variable is %s\n", ret);
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
                if (cmds->str[i] == 39)
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
                    /*else if (isdigit(cmds->str[i][j + 1]))
                        cmds->str[i] = expand_arg(cmds->str[i], j);*/
                    else
                        cmds->str[i] = expand_var(cmds->str, i + 1);
                }
                i++;
            }
        cmds = cmds->next;
    }
    return (0);
}
