#include "minishell.h"

int	execute_builtin(t_simple_cmds *cmds, int **fd, int i)
{
	int	std_in;
	int	std_out;

    //store standard streams for restoration later
	std_in = dup(STDIN_FILENO);
	std_out = dup(STDOUT_FILENO);
	if (redirections(cmds, fd) != 0)
		free_and_exit(cmds, fd, EXECUTOR_REDIRECTION_ERROR);
	//redirect in/out to relevant pipes if not first or last command
	if (i > 0)
		if (dup2(fd[i - 1][0], 0) == -1)
			free_and_exit(cmds, fd, EXECUTOR_REDIRECTION_ERROR);
	if (i < cmds->amount_of_cmds - 1)
		if (dup2(fd[i][1], 1) == -1)
			free_and_exit(cmds, fd, EXECUTOR_REDIRECTION_ERROR);
	//execute the builtin function
	cmds->builtin(cmds);
	//restore standard streams because this is the parent process
    if (!dup2(std_in, STDIN_FILENO) || !dup2(std_out, STDOUT_FILENO))
        free_and_exit(cmds, fd, EXECUTOR_REDIRECTION_ERROR);
    return (0);
}

void	child_process(t_simple_cmds *cmds, int	**fd, int i)
{
	close_unneccesary_fds(fd, i, cmds->amount_of_cmds);
    //if process is first command, input is redirected from infile or stay as stin
	//if process is last command, output is redirected to outfile or stay as stdout
	if (redirections(cmds, fd) != 0)
		free_and_exit(cmds, fd, EXECUTOR_REDIRECTION_ERROR);
	//replace stdin with read end of pipe i-1 if not
	if (cmds->index != 0)
	{
		if(dup2(fd[i - 1][0], 0) == -1)
		{
			perror("Error duplicating stdin");
            free_and_exit(cmds, fd, EXECUTOR_REDIRECTION_ERROR);
		}
	}
	//replace stdout with write part of pipe i if not
	if (cmds->index != cmds->amount_of_cmds - 1)
	{
		if(dup2(fd[i][1], 1) == -1)
         {
			perror("Error duplicating stdout");
            free_and_exit(cmds, fd, EXECUTOR_REDIRECTION_ERROR);
		}
	}
	//close remaining fds
	if (i != 0)
		close(fd[i - 1][0]);
	if (i != cmds->amount_of_cmds - 1)
		close(fd[i][1]);
	//execute command - on success, function never returns here
	execute(cmds, cmds->env, fd);
}