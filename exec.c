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

void	execute(char **cmds, char **envp)
{
	ft_printf("execute function: command is %s\n", cmds[0]);
	write(2, "\n", 1);
	if (execve(get_path(cmds[0]), cmds, envp) == -1)
	{
		ft_putstr_fd("minishell: command not found: ", 2);
		ft_putendl_fd(cmds[0], 2);
		//free_tab(s_cmd);
		exit(127);
	}
}

int	close_unneccesary_fds(int **fd, int i, int amount_of_cmds)
{
	int j;

	j = 0;
	while (j < amount_of_cmds)
	{
		if (i != j)
			close (fd[j][0]);
		if (i != j)
			close (fd[j][1]);
		j++;
	}
	return (1);
}

int	oldest_child(t_simple_cmds *cmds, int **fd)
{
	int	fd_in;

	
	if (cmds->in != NULL)
	{
		fd_in = open(cmds->in, O_RDONLY);
		if (!fd || !dup2(fd_in, 0))
		{
			//this function call will close ALL fds
			close_unneccesary_fds(fd, cmds->amount_of_cmds + 1, cmds->amount_of_cmds);
			//free cmds etc
			exit(1);
		}
	}
	else
		fd_in = STDIN_FILENO;
	ft_printf("hello from oldest child input has fd:%d\n", fd_in);
	return (fd_in);
}

int	youngest_child(t_simple_cmds *cmds, int **fd)
{
	int	fd_out;

	if (cmds->out != NULL)
	{
		fd_out = open(cmds->out, O_RDWR | O_CREAT, 0666);
		if (!fd || !dup2(fd_out, 1))
		{
			//this function call will close ALL fds
			close_unneccesary_fds(fd, cmds->amount_of_cmds + 1, cmds->amount_of_cmds);
			return (-1);
		}
	}
	else
		fd_out = STDOUT_FILENO;
	ft_printf("hello from youngest child output has fd:%d\n", fd_out);
	return (fd_out);
}

void	child_process(t_simple_cmds *cmds, int	**fd, int i)
{
	int fd_out;
	int	fd_in;

	close_unneccesary_fds(fd, i, cmds->amount_of_cmds);
	//if this process is the first command, input should be redirected from infile or stay as stin
	if (cmds->index == 0)
		fd_in = oldest_child(cmds, fd);
	//replace stdin with read end of pipe i-1 if not
	else
		dup2(fd[i - 1][0], 0);
	//if this process is the last command, output should be redirected to outfile or stay as stdout
	if (cmds->index == cmds->amount_of_cmds - 1)
		fd_out = youngest_child(cmds, fd);
	//replace stdout with write part of pipe i if not
	else
		dup2(fd[i][1], 1);
	//close remaing fds
	close(fd[i - 1][0]);
	close(fd[i][1]);
	ft_printf("child %i handled fd, ready to execute\n", i);
	//execute command
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
		pid[i] = fork();
		ft_printf("forked child %d, pid is %d\n", i, pid[i]);
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
		int		**fd;
		pid_t	pid[cmds->amount_of_cmds - 1];
		int	i;
		
		i = 0;
		fd = malloc(sizeof(int *) * cmds->amount_of_cmds);
		while (i < cmds->amount_of_cmds)
		{
			fd[i] = malloc(sizeof(int) * 2);
			i++;
		}
		//check if only one command and it is a builtin
		/*if (cmds->amount_of_cmds == 1 && cmds->builtin != NULL)
			cmd->builtin(tools, cmd);*/
		//create required amount of pipes
		if (!create_pipes(cmds->amount_of_cmds, fd))
			return (-1);
		ft_printf("created pipes\n");
		//handle heredocs here ...
		//fork required amount of processes
		if (!fork_processes(cmds, pid, fd))
			return (-1);
		ft_printf("parent: forked children\n");
		//Parent process
		if (!parent_process(cmds, fd, pid))
			return (-1);
		//free_array(fd);
		return (0);
}

t_simple_cmds *create_simple_cmds(char **str, char *in, char *out, char **envp, int index, int amount_of_cmds) {
    t_simple_cmds *cmd = malloc(sizeof(t_simple_cmds));
    if (cmd == NULL) {
        perror("malloc");
        exit(EXIT_FAILURE);
    }

    // Allocate memory and copy the string array
    int num_strings = 0;
    while (str[num_strings] != NULL) {
        num_strings++;
    }

    cmd->str = malloc((num_strings + 1) * sizeof(char *));
    if (cmd->str == NULL) {
        perror("malloc");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < num_strings; i++) {
        cmd->str[i] = strdup(str[i]);
        if (cmd->str[i] == NULL) {
            perror("strdup");
            exit(EXIT_FAILURE);
        }
    }
    cmd->str[num_strings] = NULL;

    // Allocate memory and copy the 'in' and 'out' strings
    cmd->in = (in != NULL) ? strdup(in) : NULL;
    if (in != NULL && cmd->in == NULL) {
        perror("strdup");
        exit(EXIT_FAILURE);
    }

    cmd->out = (out != NULL) ? strdup(out) : NULL;
    if (out != NULL && cmd->out == NULL) {
        perror("strdup");
        exit(EXIT_FAILURE);
    }

    // Allocate memory and copy the environment variables array

    cmd->env = envp;
    cmd->index = index;
    cmd->amount_of_cmds = amount_of_cmds;
    cmd->next = NULL;
    cmd->prev = NULL;

    return cmd;
}

int main(int argc, char **argv, char **envp)
{
	(void)argc;
	(void)argv;
    // Create the first node
    char *str1[] = {"cat", "outfile.txt", NULL};
    char *in1 = NULL;
    char *out1 = NULL;
    //char *env1[] = {envp[0], envp[1], NULL}; // Copy only two environment variables for example
    t_simple_cmds *node1 = create_simple_cmds(str1, in1, out1, envp, 0, 2);

    // Create the second node
    char *str2[] = {"sort", "-r", NULL};
    char *in2 = NULL;
    char *out2 = NULL;
   // char *env2[] = {envp[2], envp[3], NULL}; // Copy only two environment variables for example
    t_simple_cmds *node2 = create_simple_cmds(str2, in2, out2, envp, 1, 2);

    // Link the nodes
    node1->next = node2;
    node2->prev = node1;

    // Now you have two nodes with the specified data
    // You can access the data like node1->str[0], node1->str[1], etc.

    // Don't forget to free the allocated memory when you're done using it
	ft_printf("node 2 has command %s\n", node2->str[0]);
	executor(node1);
    return 0;
}