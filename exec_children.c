#include "executor.h"

void    free_and_exit(t_simple_cmds *cmds, int **fd, int exitcode)
{
    //this function call will close ALL fds
	close_unneccesary_fds(fd, cmds->amount_of_cmds + 1, cmds->amount_of_cmds);
	free_simple_commands(cmds);
    free_array(fd);
	exit(exitcode);
}

int redirect(t_simple_cmds *cmds, int **fd, int in_or_out)
{
    int file;
    int     i;
    
    file = 0;
    i = 0;
    //they loop in case multiple in/out redirections are given in one command
    //to overwrite/create files before final redirection
    if (in_or_out == 0)
    {
         while (cmds->in != NULL)
        { 
		    file = open(cmds->in, O_RDONLY);
		    if (!file || !dup2(file, 0))
		        free_and_exit(cmds, fd, EXECUTOR_REDIRECTION_ERROR);
            if (!acces(cmds->in, R_OK))
                free_and_exit(cmds, fd, EXECUTOR_ACCESS_ERROR);
            cmds->in++;
        }
    }
    if (in_or_out == 1)
    {
        while (cmds->out != NULL)
        { 
		    file = open(cmds->out[i], O_RDWR | O_CREAT, 0666);
		    if (!file || !dup2(file, 1))
                free_and_exit(cmds, fd, EXECUTOR_REDIRECTION_ERROR);
            if (!acces(cmds->out, W_OK))
                free_and_exit(cmds, fd, EXECUTOR_ACCESS_ERROR);
            cmds->out++;
        }
    }
}

int	execute_builtin(t_simple_cmds *cmds, int **fd)
{
	int	std_in;
	int	std_out;

    //store standard streams for restoration later
	std_in = dup(STDIN_FILENO);
	std_out = dup(STDOUT_FILENO);
	//redirect input if infile
	if (cmds->in != NULL)
        redirect(cmds, fd, 0);
	//redirect output if outfile
	if (cmds->out != NULL)
        redirect(cmds, fd, 1);
	//redirect in/out to relevant pipes if not first or last command
	if (cmds->index > 0)
		dup2(fd[cmds->index - 1][0], 0);
	if (cmds->index < cmds->amount_of_cmds - 1)
		dup2(fd[cmds->index][1], 1);
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
	if (cmds->index == 0)
		redirect(cmds, fd, 0);
	//replace stdin with read end of pipe i-1 if not
	else
	{
		if(!dup2(fd[i - 1][0], 0))
            free_and_exit(cmds, fd, EXECUTOR_REDIRECTION_ERROR);
	}
    //if process is last command, output is redirected to outfile or stay as stdout
	if (cmds->index == cmds->amount_of_cmds - 1)
		redirect(cmds, fd, 1);
	//replace stdout with write part of pipe i if not
	else
	{
		if(!dup2(fd[i][1], 1))
            free_and_exit(cmds, fd, EXECUTOR_REDIRECTION_ERROR);
	}
	//close remaining fds
	if (i != 0)
		close(fd[i - 1][0]);
	if (i != cmds->amount_of_cmds - 1)
		close(fd[i][1]);
	//execute command - function never returns here
	if (execute(cmds->str, cmds->env, fd) != 0)
        free_and_exit(cmds, fd, EXECUTOR_EXEC_ERROR);
}