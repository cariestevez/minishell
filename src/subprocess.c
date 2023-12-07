#include "minishell.h"

int	restore_stdstreams(int *std)
{
    if (dup2(std[0], STDIN_FILENO) < 0)
	{
		perror("dup2");
        return (1);
	}
	close(std[0]);
	if (dup2(std[1], STDOUT_FILENO) < 0)
	{
		perror("dup2");
		return (1);
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
    //store standard streams for restoration later
	std[0] = dup(STDIN_FILENO);
	std[1] = dup(STDOUT_FILENO);
	while (builtin->index != i)
		builtin = builtin->next;
	if (redirections(builtin) != 0)
		free_and_exit(shell, fd, EXECUTOR_REDIRECTION_ERROR);
	close_unneccesary_fds(fd, i, shell->amount_of_cmds);
	//redirect in/out to relevant pipes if not first or last command
	if (i > 0)
	{
		if (dup2(fd[i - 1][0], 0) < 0)
		{
			perror("dup2");
			free_and_exit(shell, fd, EXECUTOR_REDIRECTION_ERROR);
		}
	}
	if (i < shell->amount_of_cmds - 1)
	{
		if (dup2(fd[i][1], 1) < 0)
		{
			perror("dup2");
			free_and_exit(shell, fd, EXECUTOR_REDIRECTION_ERROR);
		}
	}
	builtin->builtin(shell, shell->cmds);
	if (restore_stdstreams(std) != 0)
		free_and_exit(shell, fd, STDSTREAM_RESTORE_ERROR);
    return (0);
}

void	child_process(t_shell *shell, int **fd, int i)
{
	ft_printf("aoc is %d, cmd index is %d. shell->cmds->idx is %d\n", shell->amount_of_cmds, i, shell->cmds->index);
	close_unneccesary_fds(fd, i, shell->amount_of_cmds);
	while (shell->cmds->index != i)
		shell->cmds = shell->cmds->next;
	if (redirections(shell->cmds) != 0)
		free_and_exit(shell, fd, EXECUTOR_REDIRECTION_ERROR);
	if (i != 0)
	{
		if(dup2(fd[i - 1][0], 0) == -1)
		{
			perror("dup2");
            free_and_exit(shell, fd, EXECUTOR_REDIRECTION_ERROR);
		}
	}
	if (i != shell->amount_of_cmds - 1)
	{
		if(dup2(fd[i][1], 1) == -1)
         {
			perror("dup2");
            free_and_exit(shell, fd, EXECUTOR_REDIRECTION_ERROR);
		}
	}
	if (i != 0)
		close(fd[i - 1][0]);
	if (i != shell->amount_of_cmds - 1)
		close(fd[i][1]);
	if (execute(shell->cmds, shell->env) == EXECUTOR_EXEC_ERROR)
		free_and_exit(shell, fd, EXECUTOR_EXEC_ERROR);
}