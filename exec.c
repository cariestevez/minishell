#include "parser.h"

void	free_tab(char **tab)
{
	int	i;

	i = 0;
	while (tab[i])
	{
		free(tab[i]);
		i++;
	}
}

void	execute(char **cmds, char **envp)
{
	write(2, "\n", 1);
	if (execve(get_path(cmds[0]), cmds, envp) == -1)
	{
		ft_putstr_fd("minishell: command not found: ", 2);
		ft_putendl_fd(cmds[0], 2);
		//free_tab(s_cmd);
		exit(127);
	}
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

int	parent_process(t_simple_cmds *cmds, int	**fd, pid_t *pid)
{
	int	i;

	i = 0;
	//close all file descriptors
	while (i < cmds->amount_of_cmds)
	{
		close(fd[i][1]);
		close(fd[i][0]);
		i++;
	}
	//parent waits for all children to finish
	i = 0;
	while (i < cmds->amount_of_cmds)
	{
		waitpid(pid[i], NULL, 0);
		i++;
	}
	return (1);
}

int	create_pipes(int amount_of_cmds, int **fd)
{
	int	i;
		
	i = 0;
	//create required amount of pipes
	while (i < amount_of_cmds)
	{
		if (pipe(fd[i]) < 0)
		{
			//Error on pipe
			while (i >= 0)
			{
				close(fd[i][0]);
				close(fd[i][1]);
				i--;
			}
			return (-1);
		}
		i++;
	}
	return (1);
}

int	fork_processes(t_simple_cmds *cmds, pid_t *pid, int **fd)
{
	int		i;

	i = 0;
	while (i < cmds->amount_of_cmds)
	{
		if (!cmds->builtin)
		{
			pid[i] = fork();
			//Error on fork
			if (pid[i] < 0)
			{
				i = cmds->amount_of_cmds;
				while (i >= 0)
				{
					close(fd[i][0]);
					close(fd[i][1]);
					i--;
				}
				return (-1);
			}
			//Child process
			if (pid[i] == 0)
			{
				child_process(cmds, fd, i);
				return (1);
			}
		}
		else if (cmds->builtin)
			execute_builtin(cmds, fd[i]);
		if (cmds->next != NULL)
			cmds = cmds->next;	
		i++;
	}
	return (1);
}

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

int	executor(t_simple_cmds *cmds)
{
	/*missing
	- handling multiple redirections in a row
	- check access for all files
	- exit error code handling
	- freeing everything
	- heredocs 
	- if cmds > 1 and there is (a) builtin*/
		int		**fd;
		pid_t	pid[cmds->amount_of_cmds - 1];
		int	i;
		
		i = 0;
		fd = malloc(sizeof(int *) * cmds->amount_of_cmds);
		if (!fd)
			exit(-1);
		while (i < cmds->amount_of_cmds)
		{
			fd[i] = malloc(sizeof(int) * 2);
			if (!fd[i])
				exit(-1);
			i++;
		}
		//check if only one command and it is a builtin
		/*if (cmds->amount_of_cmds == 1 && cmds->builtin != NULL)
			cmd->builtin(tools, cmd);*/
		//create required amount of pipes
		if (!create_pipes(cmds->amount_of_cmds, fd))
			return (-1);
		//handle heredocs here ...
		//fork required amount of processes
		if (!fork_processes(cmds, pid, fd))
			return (-1);
		//Parent process
		if (!parent_process(cmds, fd, pid))
			return (-1);
		//free_array(fd);
		return (0);
}