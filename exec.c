#include "minishell.h"

void	execute(t_simple_cmds *cmds, char **envp, int **fd)
{
	write(2, "\n", 1);
	ft_printf("child %d trying to get access to %s and write to %d\n", cmds->index, get_path(cmds->str[0]), STDOUT_FILENO);
	if ((get_path(cmds->str[0], envp), F_OK) != 0)
	{
		ft_putendl_fd(cmds->str[0], 2);
		ft_putstr_fd(": access failure\n", 2);
		free_and_exit(cmds, fd, EXECUTOR_EXEC_ERROR);
	}
	if (execve(get_path(cmds->str[0]), cmds->str, envp) == -1)
	{
		ft_putendl_fd(cmds->str[0], 2);
		ft_putstr_fd(": command not found\n", 2);
		free_and_exit(cmds, fd, EXECUTOR_EXEC_ERROR);
	}
}

int	**create_pipes(t_simple_cmds *cmds, int **fd)
{
	int	i;
		
	i = 0;
	fd = malloc(sizeof(int *) * cmds->amount_of_cmds);
	if (!fd)
		return (fd);
	while (i < cmds->amount_of_cmds)
	{
		fd[i] = malloc(sizeof(int) * 2);
		if (!fd[i])
			return (fd);
		i++;
	}
	i = 0;
	//create required amount of pipes
	while (i < cmds->amount_of_cmds)
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
			free_and_exit(cmds, fd, EXECUTOR_PIPE_ERROR);
		}
		i++;
	}
	return (fd);
}

int	fork_processes(t_simple_cmds *cmds, pid_t *pid, int **fd)
{
	int		i;

	i = 0;
	while (cmds != NULL && i < cmds->amount_of_cmds)
	{
		ft_printf("fork processes: i is %d, AOC is %d, cmd is %s\n", i, cmds->amount_of_cmds, cmds->str[0]);
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
				return (free_array(fd), -1);
			}
			//Child process
			if (pid[i] == 0)
				child_process(cmds, fd, i);
		}
		else if (cmds->builtin)
			execute_builtin(cmds, fd, cmds->index);
		cmds = cmds->next;	
		i++;
	}
	return (0);
}

int	executor(t_simple_cmds *cmds)
{
	/*missing
	- heredocs 
	- test +++++commands
	- test builtin scenarios*/
	int		i;
	int		**fd;
	pid_t	pid[cmds->amount_of_cmds - 1];
	
	fd = NULL;
	//check if only one command and it is a builtin
	if (cmds->amount_of_cmds == 1 && cmds->builtin != NULL)
		execute_builtin(cmds, fd, 0);
	//otherwise continue with piping and forking
	fd = create_pipes(cmds, fd);
	if (!fd)
		return (EXECUTOR_PIPE_ERROR);
	//handle heredocs here ...
	if (fork_processes(cmds, pid, fd) != 0)
		return (EXECUTOR_FORK_ERROR);
	//this function call will close ALL fds
	close_unneccesary_fds(fd, cmds->amount_of_cmds + 1, cmds->amount_of_cmds);
	//parent waits for all children to finish
	i = 0;
	while (i < cmds->amount_of_cmds)
	{
		waitpid(pid[i], NULL, 0);
		i++;
	}
	free_array(fd);
	free_simple_commands(cmds);
	return (EXECUTOR_SUCCES);
}