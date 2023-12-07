#include "minishell.h"

int	restore_stdstreams(int *std)
{
    if (dup2(std[0], STDIN_FILENO) < 0)
	{
        return (perror("dup2"), -1);
	}
	close(std[0]);
	if (dup2(std[1], STDOUT_FILENO) < 0)
	{
		return (perror("dup2"), -1);
	}
	close(std[1]);
	return (0);
}

int	execute_builtin(t_shell *shell, int **fd, int i)
{
	//std streams are exiting on error
	int				std[2];
	t_simple_cmds	*builtin;

	builtin = shell->cmds;
    ft_printf("in exec builtin 1\n");
	std[0] = dup(STDIN_FILENO);
	std[1] = dup(STDOUT_FILENO);
	ft_printf("in exec builtin 2\n");
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
	ft_printf("in exec builtin 3\n");
	if (i < shell->amount_of_cmds - 1)
	{
		if (dup2(fd[i][1], 1) < 0)
			return (perror("dup2"), -1);
	}
	ft_printf("in exec builtin 4\n");
	builtin->builtin(shell, shell->cmds);
	if (restore_stdstreams(std) != 0)
		return (-1);
    return (0);
}

int	child_process(t_shell *shell, int **fd, int i)
{
	print_simple_cmds_list(shell);
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
	//if (i != 0)
		//close(fd[i - 1][0]);
	//if (i != shell->amount_of_cmds - 1)
	//	close(fd[i][1]);
	execute(shell->cmds, shell->env);
	return (-1);
}