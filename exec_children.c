#include "parser.h"

int	execute_builtin(t_simple_cmds *cmds, int **fd)
{
	int	fd_in;
	int	fd_out;
	int	std_in;
	int	std_out;

	fd_in = 0;
	fd_out = 1;
	std_in = dup(STDIN_FILENO);
	std_out = dup(STDOUT_FILENO);
	//redirect input if infile
	if (cmds->in != NULL)
	{
		fd_in = open(cmds->in, O_RDONLY);
		if (!fd_in || !dup2(fd_in, 0))
		{
			perror("open or dup2");
			//free cmds etc
			exit(1);
		}
	}
	//redirect output if outfile
	if (cmds->out != NULL)
	{
		fd_out = open(cmds->out, O_RDWR | O_CREAT, 0666);
		if (!fd_out || !dup2(fd_out, 1))
		{
			perror("open or dup2");
			//free cmds
			return (-1);
		}
	}
	//redirect in/out to relevant pipes if not first or last command
	if (cmds->index > 0)
		dup2(fd[cmds->index - 1][0], 0);
	if (cmds->index < cmds->amount_of_cmds - 1)
		dup2(fd[cmds->index][1], 1);
	//execute the builtin function
	cmds->builtin(cmds);
	// restore standard streams because this is the parent process
    dup2(std_in, STDIN_FILENO);
    dup2(std_out, STDOUT_FILENO);
}

int	oldest_child(t_simple_cmds *cmds, int **fd)
{
	int	fd_in;
	
	fd_in = 0;
	if (cmds->in != NULL)
	{
		fd_in = open(cmds->in, O_RDONLY);
		if (!fd_in || !dup2(fd_in, 0))
		{
			perror("open or dup2");
			//this function call will close ALL fds
			close_unneccesary_fds(fd, cmds->amount_of_cmds + 1, cmds->amount_of_cmds);
			//free cmds etc
			exit(1);
		}
	}
	return (fd_in);
}

int	youngest_child(t_simple_cmds *cmds, int **fd)
{
	int	fd_out;

	fd_out = 0;
	if (cmds->out != NULL)
	{
		fd_out = open(cmds->out, O_RDWR | O_CREAT, 0666);
		if (!fd_out || !dup2(fd_out, 1))
		{
			perror("open or dup2");
			//this function call will close ALL fds
			close_unneccesary_fds(fd, cmds->amount_of_cmds + 1, cmds->amount_of_cmds);
			return (-1);
		}
	}
	return (fd_out);
}

void	child_process(t_simple_cmds *cmds, int	**fd, int i)
{
	close_unneccesary_fds(fd, i, cmds->amount_of_cmds);
	//if this process is the first command, input should be redirected from infile or stay as stin
	if (cmds->index == 0)
		oldest_child(cmds, fd);
	//replace stdin with read end of pipe i-1 if not
	else
	{
		if(!dup2(fd[i - 1][0], 0))
			perror("dup2");
	}
	//if this process is the last command, output should be redirected to outfile or stay as stdout
	if (cmds->index == cmds->amount_of_cmds - 1)
		youngest_child(cmds, fd);
	//replace stdout with write part of pipe i if not
	else
	{
		if(!dup2(fd[i][1], 1))
			perror("dup2");
	}
	//close remaining fds
	if (i != 0)
		close(fd[i - 1][0]);
	if (i != cmds->amount_of_cmds - 1)
		close(fd[i][1]);
	//execute command - function never returns here
	execute(cmds->str, cmds->env);
}