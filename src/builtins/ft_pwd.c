#include "minishell.h"

int ft_pwd(t_shell *shell, t_simple_cmds *cmd)
{
    char    path[BUFFER];
   
    (void)shell;
    (void)cmd;
    if (!getcwd(path, BUFFER))
        return (-1);
    ft_putstr_fd(path, 1);
    write(1, "\n", 1);
    return (0);
}
