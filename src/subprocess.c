#include "minishell.h"

int	execute_builtin(t_shell *shell, int **fd, int i)
{
	t_simple_cmds	*builtin;
	int				std[2];

	std[0] = dup(0);
	std[1] = dup(1);
	close_unneccesary_fds(fd, i, shell->amount_of_cmds);
	builtin = shell->cmds;
	while (builtin->index != i)
		builtin = builtin->next;
	if (fd != NULL && redirect_fds(fd, i, shell->amount_of_cmds) != 0)//redirect to pipes
		return (1);
	if (redirections(builtin) != 0)//redirect to files
		return (1);
	builtin->builtin(shell, shell->cmds);
	if (shell->amount_of_cmds == 1)
	{
		if (dup2(std[0], 0) < 0)//restore stdin
        	return (perror("dup2"), 1);
		close(std[0]);
		if (dup2(std[1], 1) < 0)//restore stdout
			return (perror("dup2"), 1);
		close(std[1]);
	}
    return (0);
}

int	child_process(t_shell *shell, int **fd, int i)
{
	close_unneccesary_fds(fd, i, shell->amount_of_cmds);
	while (shell->cmds->index != i)
		shell->cmds = shell->cmds->next;
	if (redirect_fds(fd, i, shell->amount_of_cmds) != 0)
		return (1);
	ft_printf("child calling redirections() now\n");
	if (redirections(shell->cmds) != 0)
		return (1);
	execute(shell->cmds, shell->env);
	return (1);
}