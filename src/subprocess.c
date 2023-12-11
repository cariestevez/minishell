#include "minishell.h"

int	execute_builtin(t_shell *shell, int **fd, int i)
{
	t_simple_cmds	*builtin;

	builtin = shell->cmds;
	while (builtin->index != i)
		builtin = builtin->next;
	if (redirections(builtin) != 0)
		return (-1);
	close_unneccesary_fds(fd, i, shell->amount_of_cmds);
	if (i > 0)
	{
		if (dup2(fd[i - 1][0], 0) < 0)
			return (perror("dup2"), -1);
	}
	if (i < shell->amount_of_cmds - 1)
	{
		if (dup2(fd[i][1], 1) < 0)
			return (perror("dup2"), -1);
	}
	builtin->builtin(shell, shell->cmds);
    return (0);
}

int	child_process(t_shell *shell, int **fd, int i)
{
	close_unneccesary_fds(fd, i, shell->amount_of_cmds);
	while (shell->cmds->index != i)
		shell->cmds = shell->cmds->next;
	if (redirections(shell->cmds) != 0)
		return (-1);
	if (i != 0)
	{
		if(dup2(fd[i - 1][0], 0) == -1)
            return (perror("dup2"), -1);
	}
	if (i != shell->amount_of_cmds - 1)
	{
		if(dup2(fd[i][1], 1) == -1)
			return (perror("dup2"), -1);
	}
	execute(shell->cmds, shell->env);
	return (-1);
}