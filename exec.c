#include "parser.h"

char	*get_path(char *cmd)
{
	int		i;
	char	*exec;
	char	**fullpath;
	char	*path_elem;
	char	**s_cmd;

	i = 0;
	fullpath = ft_split(getenv("PATH"), ':');
	s_cmd = ft_split(cmd, ' ');
	while (fullpath[i])
	{
		path_elem = ft_strjoin(fullpath[i], "/");
		exec = ft_strjoin(path_elem, s_cmd[0]);
		free(path_elem);
		if (access(exec, F_OK | X_OK) == 0)
		{
			ft_free_tab(s_cmd);
			return (exec);
		}
		free(exec);
		i++;
	}
	ft_free_tab(fullpath);
	ft_free_tab(s_cmd);
	return (cmd);
}

void	execute(char *cmd, char **env)
{
	char	**s_cmd;
	char	*path;	

	s_cmd = ft_split(cmd, ' ');
	write(2, "\n", 1);
	path = get_path(s_cmd[0]);
	if (execve(path, s_cmd, env) == -1)
	{
		ft_putstr_fd("minishell: command not found: ", 2);
		ft_putendl_fd(s_cmd[0], 2);
		ft_free_tab(s_cmd);
		exit(127);
	}
}

int	child_process(t_simple_commands *cmds, int	**fd, int i)
{
	int j;
	int fd_out;
	//close all unneccesary file descriptors
	j = 0;
	while (j < amount_of_commands + 1)
	{
		if (i != j)
			close (fd[j][0]);
		if (i + 1 != j)
			close (fd[j][1]);
		j++;
	}
	//replace stdin with read end of pipe i
	dup2(fd[i][0], 0);
	//if this process is the last command, output should be redirected to outfile or stay as stdout
	if (cmd_index == amount_of_commands - 1)
	{
		if (redirections->out != NULL)
			dup2(redirections->out, 1);
	}
	//replace stdout with write part of pipe i+1 if not
	else
		dup2(fd[i+1][1], 1);
	//read from fd[i][0]
	execute(get_path(cmds->str[0]), env);
	//write to fd[i + 1][1]
	close(fd[i][0]);
	close(fd[i + 1][1]);
	return (1);
}

int	parent_process(t_simple_commands *cmds, int	**fd, int *pid, int i)
{
	//close all unneccesary file descriptors
	while (i > 0)
	{
		close(fd[i][1])
		if (i != amount_of_commands)
			close(fd[i][0]);
		i--;
	}
	//the parent will handle the input and send it to the first child via pipe fd[0]
	//read from infile if given
	if (redirections->in != NULL)
		dup2(redirections->in, 0);
	dup2(fd[0][1], 1);
	//write (input for first command) to fd[0][1]
	close(fd[0][1]);
	close(fd[i][0]);
	//parent waits for all children to finish
	i = 0;
	while (i < amount_of_commands)
	{
		waitpid(pid[i], NULL, 0);
		i++;
	}
	return (1);
}

int	create_pipes(int amount_of_commands, int **fd)
{
	int	i;
		
	i = 0;
	//create required amount of pipes
	while (i < amount_of_commands + 1)
	{
		if (pipe(fd[i]) < 0)
		{
			//Error on pipe
			while (i >= 0)
			{
				close(fd[i]);
				i--;
			}
			return (-1);
		}
		i++;
	}
	return (1);
}

int	fork_processes(t_simple_commands *cmds, int *pid, int **fd)
{
	int	i;

	i = 0;
	while (i < amount_of_commands)
	{
		pid[i] = fork;
		//Error on fork
		if (pid[i] < 0)
		{
			i = amount_of_commands - 1;
			while (i >= 0)
			{
				close(fd[i]);
				i--;
			}
			return (-1);
		}
		//Child process
		if (pid == 0)
		{
			if (!child_process(cmds, fd, i))
				return (-1);
			return (1);
		}
		cmds = cmds->next;	
		i++;
	}
	return (1);
}

int	executor(t_simple_commands *cmds)
{
		int	fd[amount_of_commands + 1][2];
		int pid[amount_of_commands];
		int	i;

		i = 0;
		//check if only one command and it is a builtin
		if (amount_of_commands == 1 && cmd->builtin != NULL)
			cmd->builtin(tools, cmd);
		//create required amount of pipes
		if (!create_pipes(amount_of_commands, fd))
			return (-1);
		//handle heredocs here ...
		//fork required amount of processes
		if (!fork_processes(cmds, pid, fd))
			return (-1);
		//Parent process
		if (!parent_process(cmds, fd, i, pid))
			return (-1);
		return (0);
}