#include "../minishell.h"

int ft_pwd(void)
{
    char    path[BUFFER];
   
    if (!getcwd(path, BUFFER))
        return (-1);
    ft_putstr_fd(path, 1);
    write(1, "\n", 1);
    return (0);
}
