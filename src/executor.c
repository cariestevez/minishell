#include "minishell.h"

int	execute(t_simple_cmds *cmd, char **envp)
{
	write(2, "\n", 1);
	if ((get_path(cmd->str[0], envp), F_OK) != 0)
	{
		ft_putstr_fd("minishell: ", 1);
		ft_putstr_fd(cmd->str[0], 2);
		ft_putstr_fd(": access failure\n", 2);
		return (-1);
	}
	if (execve(get_path(cmd->str[0], envp), cmd->str, envp) == -1)
	{
		ft_putstr_fd("minishell: ", 1);
		ft_putstr_fd(cmd->str[0], 2);
		ft_putstr_fd(": command not found\n", 2);
	}
	return (-1);
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
				i = shell->amount_of_cmds;
				while (i >= 0)
				{
					close(fd[i][0]);
					close(fd[i][1]);
					i--;
				}
				return (-1);
			}
			if (pid[i] == 0)
			{
				child_process(shell, fd, i);
				return (-1);
			}
		}
		else if (shell->cmds->builtin)
		{
			if (execute_builtin(shell, fd, i))
				return (-1);
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
	t_simple_cmds *head;

	head = shell->cmds;
	fd = NULL;
	if (shell->amount_of_cmds == 1 && shell->cmds->builtin != NULL)
			return (execute_builtin(shell, fd, 0));
	fd = create_pipes(shell, fd);
	if (!fd)
		return (free_and_exit(shell, fd));
	if (fork_processes(shell, pid, fd) != 0)
		return (free_and_exit(shell, fd), errno + 1000);
	//this call will close all fds
	close_unneccesary_fds(fd, shell->amount_of_cmds + 1, shell->amount_of_cmds);
	i = 0;
	while (i < shell->amount_of_cmds)
	{
		while (head->index != i)
			head = head->next;
		if (head->builtin != NULL)
			status = 0;
		else
		{
			waitpid(pid[i], &status, 0);
			status =  WEXITSTATUS(status);
		}
		i++;
	}
	free_i_array(fd);
	return (status);
}
