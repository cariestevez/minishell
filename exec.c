#include "parser.h"
int	free_tab(char **tab)
{
	int	i;

	i = 0;
	while (tab[i])
	{
		free(tab[i]);
		i++;
	}
}

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
			free_tab(s_cmd);
			return (exec);
		}
		free(exec);
		i++;
	}
	free_tab(fullpath);
	free_tab(s_cmd);
	return (cmd);
}

void	execute(char *cmd, char **envp)
{
	char	**s_cmd;
	char	*path;	

	s_cmd = ft_split(cmd, ' ');
	write(2, "\n", 1);
	path = get_path(s_cmd[0]);
	if (execve(path, s_cmd, envp) == -1)
	{
		ft_putstr_fd("minishell: command not found: ", 2);
		ft_putendl_fd(s_cmd[0], 2);
		free_tab(s_cmd);
		exit(127);
	}
}

int	child_process(t_simple_cmds *cmds, int	**fd, int i)
{
	int j;
	int fd_out;
	int	fd_in;

	//close all unneccesary file descriptors
	j = 0;
	while (j < cmds->amount_of_cmds + 1)
	{
		if (i != j)
			close (fd[j][0]);
		if (i + 1 != j)
			close (fd[j][1]);
		j++;
	}
	//replace stdin with read end of pipe i
	fd_in = open(fd[i][0]);
	dup2(fd[i][0], 0);
	//if this process is the last command, output should be redirected to outfile or stay as stdout
	if (cmds->index == cmds->amount_of_cmds - 1)
	{
		if (cmds->out != NULL)
			fd_out = open(cmds->out, 1);
	}
	//replace stdout with write part of pipe i+1 if not
	else
		fd_out = open(fd[i + 1][1], 1);
	dup2(fd_out, 1);
	//close remaing fds
	close(fd[i][0]);
	close(fd[i + 1][1]);
	//execute command
	execute(get_path(cmds->str[0]), cmds->env);
	return (1);
}

int	parent_process(t_simple_cmds *cmds, int	**fd, int *pid, int i)
{
	int	fd_in;
	int	fd_out;
	//close all unneccesary file descriptors
	while (i > 0)
	{
		close(fd[i][1]);
		if (i != cmds->amount_of_cmds)
			close(fd[i][0]);
		i--;
	}
	//the parent will handle the input and send it to the first child via pipe fd[0]
	//read from infile if given
	if (cmds->in != NULL)
		fd_in = open(cmds->in);
	else	
		fd_in = 
	dup2(fd_in, 0);
	fd_out = fd[0][1];
	dup2(fd_out, 1);
	//write (input for first command) to fd[0][1]
	close(fd[0][1]);
	close(fd[i][0]);
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
	while (i < amount_of_cmds + 1)
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

int	fork_processes(t_simple_cmds *cmds, int *pid, int **fd)
{
	int		i;

	i = 0;
	while (i < cmds->amount_of_cmds)
	{
		pid[i] = fork;
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

int	count_commands(t_simple_cmds *cmds)
{
	int		i;

	i = 0;
	while (cmds != NULL)
	{
		cmds->index = i;
		i++;
		cmds = cmds->next;
	}
	return (i);
}

int	executor(t_simple_cmds *cmds)
{
		int	fd[cmds->amount_of_cmds + 1][2];
		int pid[cmds->amount_of_cmds];
		int	i;

		i = 0;
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
		if (!parent_process(cmds, fd, i, pid))
			return (-1);
		return (0);
}

int	main(int argc, char **argv, char **envp)
{
	t_simple_cmds		*cmds;
	char				*str1 = "ls -l";
	char				*str2 = "grep exec.c";
	char				*in = NULL;
	char				*out = "outfile.txt";


	cmds = malloc(sizeof(t_simple_cmds) * 2);
	cmds->str = ft_split(str1, ' ');
	cmds->in = NULL;
	cmds->out = NULL;
	cmds->env = envp;
	cmds->amount_of_cmds = count_commands(cmds);
	cmds = cmds->next;
	cmds->str = ft_split(str2, ' ');
	cmds->in = in;
	cmds->out = out;
	cmds->env = envp;
	cmds->amount_of_cmds = count_commands(cmds);
	executor(cmds);
}