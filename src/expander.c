#include "minishell.h"

int get_expanded_variable(char **str, int start, int end, t_shell *shell)
{ 
    char    *var;
    char    *tmp;
    char    *ret;
    char    *append;
    int     new_index;
    
    if ((*str)[start] == '$' && (*str)[start + 1] == '?')
        var = ft_itoa(shell->exitcode);
    else
    {
        var = ft_substr(*str, start, end - start);
        tmp = ft_strtrim(var, "}${"); 
        free(var);
        var = ft_getenv(tmp, shell->env);
        if (!var)
            var = "";
         free(tmp);
    }
    ret = ft_substr(*str, 0, start);
    tmp = ft_strjoin(ret, var);
    free(ret);
    new_index = ft_strlen(tmp);
    append = ft_substr(*str, end, ft_strlen(*str) - end + 1);
    free(*str);
    *str = ft_strjoin(tmp, append);
    free(tmp);
    free(append);
    return (new_index);
}

char    *check_for_variables(char *str, t_shell *shell)
{
    int     i;
    int     start;
    int     q_flag;
   
    i = 0;
    start = 0;
    q_flag = -1;
    while (str[i] != '\0')
    {
        if (str[i] == '\"')
            q_flag += -1;
        if (str[i] == '\'' && q_flag == -1)
        {
            i++;
            while (str[i] != '\0' && str[i] != '\'')
                i++;
        }
        if  (str[i] == '$' && str[i + 1] == '?')
        {
            i = get_expanded_variable(&str, i, i + 2, shell) - 1;
            if (!str)
                return (NULL);
        }
        if (str[i] == '$' && str[i + 1] != '\0' && (ft_isalnum(str[i + 1]) == 1 || ft_strchr("_{}", str[i + 1]) != 0))
        {
            start = i;
            i++;
            while (str[i] != '\0' && (ft_isalnum(str[i]) == 1 || ft_strchr("_{}", str[i]) != 0))
                i++;
            i = get_expanded_variable(&str, start, i, shell) - 1;
            if (!str)
                return (NULL);
        }
        i++;
    }
    return (str);
}

int expander(t_shell *shell)
{
    t_simple_cmds *head;
    char        *tmp;
    int            i;

    head = shell->cmds;
    i = 0;
    while (shell->cmds != NULL)
    {
        i = 0;
        while (shell->cmds->str[i] != NULL)
        {       
            shell->cmds->str[i] = check_for_variables(shell->cmds->str[i], shell);
            if (shell->cmds->str[i] == NULL)
            {
                tmp = shell->cmds->str[i];
                shell->cmds->str[i] = shell->cmds->str[i + 1];
                free(tmp);
            }
            quote_removal(shell, i);
            i++;
        }
        shell->cmds = shell->cmds->next;
    }
    shell->cmds = head;
    return (0);
}
