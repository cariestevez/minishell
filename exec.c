#include "minishell.h"

int	execute(t_simple_cmds *cmd, char **envp)
{
	write(2, "\n", 1);
	ft_printf("child %d trying to get access to %s and write to %d\n", cmd->index, get_path(cmd->str[0], envp), STDOUT_FILENO);
	if ((get_path(cmd->str[0], envp), F_OK) != 0)
	{
		ft_putendl_fd(cmd->str[0], 2);
		ft_putstr_fd(": access failure\n", 2);
		return (EXECUTOR_EXEC_ERROR);
	}
	if (execve(get_path(cmd->str[0], envp), cmd->str, envp) == -1)
	{
		ft_putendl_fd(cmd->str[0], 2);
		ft_putstr_fd(": command not found\n", 2);
	}
	return (EXECUTOR_EXEC_ERROR);
}

int	**create_pipes(t_shell *shell, int **fd)
{
	int	i;

	i = 0;
	fd = malloc(sizeof(int *) * shell->amount_of_cmds);
	if (!fd)
		return (fd);
	while (i < shell->amount_of_cmds)
	{
		fd[i] = malloc(sizeof(int) * 2);
		if (!fd[i])
			return (fd);
		i++;
	}
	i = 0;
	//create required amount of pipes
	while (i < shell->amount_of_cmds)
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
			free_and_exit(shell, fd, EXECUTOR_PIPE_ERROR);
		}
		i++;
	}
	return (fd);
}

int	fork_processes(t_shell *shell, pid_t *pid, int **fd)
{
	int		i;

	i = 0;
	while (shell->cmds != NULL && i < shell->amount_of_cmds)
	{
		if (!shell->cmds->builtin)
		{
			pid[i] = fork();
			//Error on fork
			if (pid[i] < 0)
			{
				i = shell->amount_of_cmds;
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
				child_process(shell, fd, i);
		}
		else if (shell->cmds->builtin)
			execute_builtin(shell, fd, shell->cmds->index);
		shell->cmds = shell->cmds->next;
		i++;
	}
	return (0);
}

int	executor(t_shell *shell)
{
	/*missing
	- child is not waiting for pipe input, are redirections ok?
	fx. cat <<EOF | ls -l
	- test +++++commands
	- test builtin scenarios*/
	int		i;
	int		**fd;
	pid_t	pid[shell->amount_of_cmds - 1];

	fd = NULL;
	//check if only one command and it is a builtin
	if (shell->amount_of_cmds == 1 && shell->cmds->builtin != NULL)
		execute_builtin(shell, fd, 0);
	//otherwise continue with piping and forking
	fd = create_pipes(shell, fd);
	if (!fd)
		return (EXECUTOR_PIPE_ERROR);
	//handle heredocs here ...
	if (fork_processes(shell, pid, fd) != 0)
		return (EXECUTOR_FORK_ERROR);
	//this function call will close ALL fds
	close_unneccesary_fds(fd, shell->amount_of_cmds + 1, shell->amount_of_cmds);
	//parent waits for all children to finish
	i = 0;
	while (i < shell->amount_of_cmds)
	{
		waitpid(pid[i], NULL, 0);
		i++;
	}
	free_array(fd);
	free_simple_commands(shell->cmds);
	return (SUCCESS);
}
