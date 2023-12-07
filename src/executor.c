#include "minishell.h"

int	execute(t_simple_cmds *cmd, char **envp)
{
	write(2, "\n", 1);
	if ((get_path(cmd->str[0], envp), F_OK) != 0)
	{
		ft_putstr_fd(cmd->str[0], 2);
		ft_putstr_fd(": access failure\n", 2);
		return (EXECUTOR_EXEC_ERROR);
	}
	if (execve(get_path(cmd->str[0], envp), cmd->str, envp) == -1)
	{
		ft_putstr_fd(cmd->str[0], 2);
		ft_putstr_fd(": command not found\n", 2);
	}
	return (EXECUTOR_EXEC_ERROR);
}

int	**create_pipes(t_shell *shell, int **fd)
{
	int	i;

	i = 0;
	fd = ft_calloc(sizeof(int *), shell->amount_of_cmds + 1);
	if (!fd)
		return (fd);
	while (i < shell->amount_of_cmds)
	{
		fd[i] = ft_calloc(sizeof(int), 2);
		if (!fd[i])
			return (fd);
		i++;
	}
	i = 0;
	while (i < shell->amount_of_cmds)
	{
		if (pipe(fd[i]) < 0)
		{
			perror("pipe");
			while (i >= 0)
			{
				close(fd[i][0]);
				close(fd[i][1]);
				i--;
			}
			return (NULL);
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
			if (pid[i] < 0)
			{
				perror("fork");
				i = shell->amount_of_cmds;
				while (i >= 0)
				{
					close(fd[i][0]);
					close(fd[i][1]);
					i--;
				}
				return (EXECUTOR_FORK_ERROR);
			}
			if (pid[i] == 0)
			{
				ft_printf("aoc is %d, cmd index is %d\n", shell->amount_of_cmds, i);
				child_process(shell, fd, i);
			}
		}
		else if (shell->cmds->builtin)
		{
			if (execute_builtin(shell, fd, shell->cmds->index))
				return (EXECUTOR_EXEC_ERROR);
		}
		shell->cmds = shell->cmds->next;
		i++;
	}
	return (0);
}

int	executor(t_shell *shell)
{
	int		i;
	int		**fd;
	pid_t	pid[shell->amount_of_cmds];
	int		status;

	fd = NULL;
	if (shell->amount_of_cmds == 1 && shell->cmds->builtin != NULL)
	{
		if(execute_builtin(shell, fd, 0) == 0)
			return (SUCCESS);
	}
	fd = create_pipes(shell, fd);
	if (!fd)
		return (free_and_exit(shell, fd, EXECUTOR_PIPE_ERROR));
	if (fork_processes(shell, pid, fd) != 0)
		return (free_and_exit(shell, fd, EXECUTOR_FORK_ERROR));
	//this call will close all fds
	close_unneccesary_fds(fd, shell->amount_of_cmds + 1, shell->amount_of_cmds);
	i = 0;
	while (i < shell->amount_of_cmds)
	{
		waitpid(pid[i], &status, 0);
		if (WIFEXITED(status) < 1)
			return (status);
		i++;
	}
	free_i_array(fd);
	return (SUCCESS);
}
