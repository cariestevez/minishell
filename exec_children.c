#include "minishell.h"

int	execute_builtin(t_shell *shell, int **fd, int i)
{
	int	std_in;
	int	std_out;

    //store standard streams for restoration later
	std_in = dup(STDIN_FILENO);
	std_out = dup(STDOUT_FILENO);
	while (shell->cmds->index != i)
		shell->cmds = shell->cmds->next;
	if (redirections(shell->cmds) != 0)
		free_and_exit(shell, fd, EXECUTOR_REDIRECTION_ERROR);
	//redirect in/out to relevant pipes if not first or last command
	if (i > 0)
	{
		if (dup2(fd[i - 1][0], 0) == -1)
			free_and_exit(shell, fd, EXECUTOR_REDIRECTION_ERROR);
	}
	if (i < shell->amount_of_cmds - 1)
	{
		if (dup2(fd[i][1], 1) == -1)
			free_and_exit(shell, fd, EXECUTOR_REDIRECTION_ERROR);
	}
	//execute the builtin function
	shell->cmds->builtin(shell, shell->cmds);
	//restore standard streams because this is the parent process
    if (!dup2(std_in, STDIN_FILENO) || !dup2(std_out, STDOUT_FILENO))
        free_and_exit(shell, fd, EXECUTOR_REDIRECTION_ERROR);
    return (0);
}

void	child_process(t_shell *shell, int **fd, int i)
{
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