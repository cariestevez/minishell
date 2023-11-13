#include "parser.h"

int pwd(t_simple_cmds *cmds)
{
    char    path[BUFFER];
   
    if (getcwd(path, BUFFER) == NULL)
        return (-1);
    ft_putstr_fd(path, 1);
    return (0);
}
