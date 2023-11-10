#include "minishell.h"

int redirect(t_redir *redir)
{
    int 	file;
    
	ft_printf("hello from redirect, first node is %s\n", redir->str);
    file = -1;
    //they loop in case multiple in/out redirections are given in one command
    //to overwrite/create files before final redirection
    while (redir != NULL)
    {
		ft_printf("redir is %s\n", redir->str);
			if (redir->type == l_in)
    		{
		    	file = open(redir->str, O_RDONLY);
		    	if (file == -1|| dup2(file, 0) == -1 || access(redir->str, R_OK) == -1)
		        	return (EXECUTOR_REDIRECTION_ERROR);
        	}
			else
			{
				if (redir->type == l_append)
					file = open(redir->str, O_WRONLY | O_CREAT | O_APPEND, 0777);
				else if (redir->type == l_out)
		    		file = open(redir->str, O_WRONLY | O_CREAT | O_TRUNC, 0777);
		    	if (file == -1 || dup2(file, 1) == -1 || access(redir->str, W_OK) == -1)
					return (EXECUTOR_REDIRECTION_ERROR);
			}
		redir = redir->next;
    }
	return (EXECUTOR_SUCCES);
}

int	execute_builtin(t_simple_cmds *cmds, int **fd, int i)
{
	int	std_in;
	int	std_out;

    //store standard streams for restoration later
	std_in = dup(STDIN_FILENO);
	std_out = dup(STDOUT_FILENO);
	if (redirect(cmds->redir) != 0)
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
	ft_printf("hello from child process %d\n", cmds->index);
	close_unneccesary_fds(fd, i, cmds->amount_of_cmds);
    //if process is first command, input is redirected from infile or stay as stin
	//if process is last command, output is redirected to outfile or stay as stdout
	printf("Before redirection - stdin file descriptor: %d at child %d\n", fileno(stdin), cmds->index);
	//if (redirect(cmds->redir) != 0)
	//	free_and_exit(cmds, fd, EXECUTOR_REDIRECTION_ERROR);
	//replace stdin with read end of pipe i-1 if not
	if (cmds->index != 0)
	{
		if(dup2(fd[i - 1][0], 0) == -1)
		{
			perror("Error duplicating stdin");
            free_and_exit(cmds, fd, EXECUTOR_REDIRECTION_ERROR);
		}
	}
	ft_printf("hello from child process %d passed reading fd\n", cmds->index);
	//replace stdout with write part of pipe i if not
	if (cmds->index != cmds->amount_of_cmds - 1)
	{
		if(dup2(fd[i][1], 1) == -1)
         {
			perror("Error duplicating stdout");
            free_and_exit(cmds, fd, EXECUTOR_REDIRECTION_ERROR);
		}
	}
	printf("After redirection - stdin file descriptor: %d at child %d\n", fileno(stdin), cmds->index);
	//close remaining fds
	if (i != 0)
		close(fd[i - 1][0]);
	if (i != cmds->amount_of_cmds - 1)
		close(fd[i][1]);
	ft_printf("child process %d ready to exec, \n", cmds->index);
	//execute command - on success, function never returns here
	if (execute(cmds->str, cmds->env) != 0)
        free_and_exit(cmds, fd, EXECUTOR_EXEC_ERROR);
}