#include "minishell.h"

int	restore_stdstreams(int *std)
{
    if (dup2(std[0], STDIN_FILENO) < 0)
	{
		ft_printf("std_in is %d, STDIN_FILENO is %d\n", std[0], STDIN_FILENO);
        return (1);
	}
	close(std[0]);
	if (dup2(std[1], STDOUT_FILENO) < 0)
	{		
		ft_printf("std_out is %d, STDOUT_FILENO is %d\n", std[1], STDOUT_FILENO);
		return (1);
	}
	close(std[1]);
	return (0);
}

int	execute_builtin(t_shell *shell, int **fd, int i)
{
	//std streams are exiting on error
	printf("hello from exec_builtin\n");
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
			free_and_exit(shell, fd, EXECUTOR_REDIRECTION_ERROR);
	}
	if (i < shell->amount_of_cmds - 1)
	{
		if (dup2(fd[i][1], 1) < 0)
			free_and_exit(shell, fd, EXECUTOR_REDIRECTION_ERROR);
	}
	//execute the builtin function
	builtin->builtin(shell, shell->cmds);
	//restore standard streams because this is the parent process
	if (restore_stdstreams(std) != 0)
		free_and_exit(shell, fd, STDSTREAM_RESTORE_ERROR);
    return (0);
}

void	child_process(t_shell *shell, int **fd, int i)
{
	//update the non-inhereted local variables and close fds
	close_unneccesary_fds(fd, i, shell->amount_of_cmds);
    //if process is first command, input is redirected from infile or stay as stin
	//if process is last command, output is redirected to outfile or stay as stdout
	while (shell->cmds->index != i)
		shell->cmds = shell->cmds->next;
	if (redirections(shell->cmds) != 0)
		free_and_exit(shell, fd, EXECUTOR_REDIRECTION_ERROR);
	//replace stdin with read end of pipe i-1 if not
	if (i != 0)
	{
		if(dup2(fd[i - 1][0], 0) == -1)
		{
			perror("Error duplicating stdin");
            free_and_exit(shell, fd, EXECUTOR_REDIRECTION_ERROR);
		}
	}
	//replace stdout with write part of pipe i if not
	if (i != shell->amount_of_cmds - 1)
	{
		if(dup2(fd[i][1], 1) == -1)
         {
			perror("Error duplicating stdout");
            free_and_exit(shell, fd, EXECUTOR_REDIRECTION_ERROR);
		}
	}
	//close remaining fds
	if (i != 0)
		close(fd[i - 1][0]);
	if (i != shell->amount_of_cmds - 1)
		close(fd[i][1]);
	//execute command - on success, function never returns here
	if (execute(shell->cmds, shell->env) == EXECUTOR_EXEC_ERROR)
		free_and_exit(shell, fd, EXECUTOR_EXEC_ERROR);
}