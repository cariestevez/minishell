/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emollebr <emollebr@student.42berlin.d      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/13 21:21:13 by emollebr          #+#    #+#             */
/*   Updated: 2023/12/13 21:21:15 by emollebr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	pipe_fds(int **fd, int amount_of_cmds)
{
	int	i;

	i = 0;
	while (i < amount_of_cmds)
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
			return (1);
		}
		i++;
	}
	return (0);
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
	if (pipe_fds(fd, shell->amount_of_cmds) != 0)
		return (NULL);
	return (fd);
}

void	handle_fork_error(int **fd, int i)
{
	while (i >= 0)
	{
		close(fd[i][0]);
		close(fd[i][1]);
		i--;
	}
	return ;
}

int	fork_processes(t_shell *shell, pid_t *pid, int **fd)
{
	int		i;

	i = 0;
	while (shell->cmds != NULL && i < shell->amount_of_cmds)
	{
		pid[i] = fork();
		if (pid[i] < 0)
			return (handle_fork_error(fd, shell->amount_of_cmds), -1);
		if (pid[i] == 0 && !shell->cmds->builtin)
		{
			child_process(shell, fd, i);
			return (-1);
		}
		else if (pid[i] == 0 && shell->cmds->builtin)
			return (execute_builtin(shell, fd, i) + 1000);
		shell->cmds = shell->cmds->next;
		i++;
	}
	return (0);
}

int	executor(t_shell *shell)
{
	int				**fd;
	int				status;
	pid_t			*pid;
	t_simple_cmds	*head;

	fd = NULL;
	head = shell->cmds;
	if (shell->amount_of_cmds == 1 && shell->cmds->builtin != NULL)
		return (execute_builtin(shell, fd, 0), errno);
	pid = ft_calloc(sizeof(pid_t), shell->amount_of_cmds);
	fd = create_pipes(shell, fd);
	if (!fd)
		return (free_and_exit(shell, fd, pid));
	if (fork_processes(shell, pid, fd) != 0)
		return (free_and_exit(shell, fd, pid), errno + 1000);
	close_unneccesary_fds(fd, shell->amount_of_cmds + 1, shell->amount_of_cmds);
	status = wait_for_children(head, pid, shell->amount_of_cmds);
	free_int_arr(fd);
	free(pid);
	return (status);
}
