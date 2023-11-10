#include "minishell.h"

int	execute(char **cmds, char **envp)
{
	ft_printf("hello from execute, command is %s\n", cmds[0]);
	write(2, "\n", 1);
	execve(get_path(cmds[0]), cmds, envp);
	ft_putendl_fd(cmds[0], 2);
	ft_putstr_fd(": command not found\n", 2);
	return (EXECUTOR_EXEC_ERROR);
}

int	parent_process(t_simple_cmds *cmds, int	**fd, pid_t *pid)
{
	int	i;

	ft_printf("hello from parent process\n");
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
	return (0);
}

int	create_pipes(int amount_of_cmds, int **fd)
{
	int	i;
	
	ft_printf("hello from create_pipes\n");
	i = 0;
	//create required amount of pipes
	while (i < amount_of_cmds - 1)
	{
		if (pipe(fd[i]) < 0)
		{
			//Error on pipe
			while (--i >= 0)
			{
				close(fd[i][0]);
				close(fd[i][1]);
			}
			return (free_array(fd), EXECUTOR_PIPE_ERROR);
		}
		i++;
	}
	return (0);
}

int	fork_processes(t_simple_cmds *cmds, pid_t *pid, int **fd)
{
	int		i;

	ft_printf("hello from fork_processes\n");
	i = 0;
	while (i < cmds->amount_of_cmds)
	{
		if (!cmds->builtin)
		{
			pid[i] = fork();
			//Error on fork
			if (pid[i] < 0)
				free_and_exit(cmds, fd, EXECUTOR_FORK_ERROR);
			//Child process
			if (pid[i] == 0)
				child_process(cmds, fd, i);
		}
		else if (cmds->builtin)
			execute_builtin(cmds, fd, i);
		if (cmds->next != NULL)
			cmds = cmds->next;	
		i++;
	}
	return (0);
}

int	executor(t_simple_cmds *cmds)
{
	/*missing
	- handling multiple redirections in a row 
	- heredocs 
	- test builtin scenarios
	- append output to a file*/
		int		**fd;
		pid_t	pid[cmds->amount_of_cmds];
		int	i;
		
		i = 0;
		fd = malloc(sizeof(int *) * cmds->amount_of_cmds - 1);
		if (!fd)
			return (EXECUTOR_MALLOC_ERROR);
		while (i < cmds->amount_of_cmds)
		{
			fd[i] = malloc(sizeof(int) * 2);
			if (!fd[i])
				return (EXECUTOR_MALLOC_ERROR);
			i++;
		}
		//check if only one command and it is a builtin
		if (cmds->amount_of_cmds == 1 && cmds->builtin != NULL)
			execute_builtin(cmds, fd, i);
		if (create_pipes(cmds->amount_of_cmds, fd) != 0)
			return (EXECUTOR_PIPE_ERROR);
		//handle heredocs here ...
		if (fork_processes(cmds, pid, fd) != 0)
			return (EXECUTOR_FORK_ERROR);
		parent_process(cmds, fd, pid);
		free_array(fd);
		return (EXECUTOR_SUCCES);
}