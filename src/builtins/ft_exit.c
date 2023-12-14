#include "minishell.h"

int	ft_exit(t_shell *shell, t_simple_cmds *cmd)
{
    (void)cmd;
    free_cmds(shell->cmds);
    ft_putstr_fd("exit", 1);
    free_char_arr(shell->env);
	free(shell);
	rl_clear_history();
    exit(errno);
}
