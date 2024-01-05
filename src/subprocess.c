/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   subprocess.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emollebr <emollebr@student.42berlin.d      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/13 21:19:58 by emollebr          #+#    #+#             */
/*   Updated: 2023/12/13 21:20:00 by emollebr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	execute(t_simple_cmds *cmd, char **envp)
{
	if (cmd->str != NULL && cmd->str[0] != NULL && 
		execve(get_path(cmd->str[0], envp), cmd->str, envp) == -1)
	{
		ft_putstr_fd("minishell: ", 1);
		ft_putstr_fd(cmd->str[0], 2);
		ft_putstr_fd(": command not found\n", 2);
		g_last_exit = 127;
	}
	return (-1);
}

int	redirections(t_simple_cmds *cmd)
{
	t_redir	*head;

	head = cmd->redir;
	while (cmd->redir != NULL)
	{
		if (cmd->redir->type == l_in && cmd->redir->str != NULL)
		{
			if (redirect_input(cmd->redir) != 0)
				return (-1);
		}
		if ((cmd->redir->type == l_append || cmd->redir->type == l_out)
			&& cmd->redir->str != NULL)
		{
			if (redirect_output(cmd->redir) != 0)
				return (-1);
		}
		if (cmd->redir->type == l_heredoc && cmd->redir->str != NULL)
		{
			if (heredoc(cmd->redir, cmd->index) != 0)
			{
				ft_putstr_fd("enters here\n", 2);
				return (-1);
			}
		}
		cmd->redir = cmd->redir->next;
	}
	cmd->redir = head;
	return (0);
}

int	execute_builtin(t_shell *shell, int **fd, int i)
{
	t_simple_cmds	*builtin;
	int				std[2];

	std[0] = dup(0);
	std[1] = dup(1);
	close_unneccesary_fds(fd, i, shell->amount_of_cmds);
	builtin = shell->cmds;
	while (builtin->index != i)
		builtin = builtin->next;
	builtin->redir = builtin->redir_head;
	if (fd != NULL && redirect_fds(fd, i, shell->amount_of_cmds) != 0)
		return (1);
	if (redirections(builtin) != 0)
		return (1);
	builtin->builtin(shell, shell->cmds);
	if (shell->amount_of_cmds == 1)
	{
		if (dup2(std[0], 0) < 0)
			return (perror("dup2"), 1);
		close(std[0]);
		if (dup2(std[1], 1) < 0)
			return (perror("dup2"), 1);
		close(std[1]);
	}
	return (0);
}

int	child_process(t_shell *shell, int **fd, int i)
{
	close_unneccesary_fds(fd, i, shell->amount_of_cmds);
	while (shell->cmds->index != i)
		shell->cmds = shell->cmds->next;
	shell->cmds->redir = shell->cmds->redir_head;
	if (redirect_fds(fd, i, shell->amount_of_cmds) != 0)
		return (1);
	if (redirections(shell->cmds) != 0)
		return (1);
	execute(shell->cmds, shell->env);
	return (1);
}

int	wait_for_children(t_simple_cmds *head, pid_t *pid, int amount_of_cmds)
{
	int	status;
	int	i;

	i = 0;
	while (i < amount_of_cmds)
	{
		while (head->index != i)
			head = head->next;
		if (head->builtin != NULL)
			status = 0;
		else
		{
			waitpid(pid[i], &status, 0);
			status = WEXITSTATUS(status);
		}
		i++;
	}
	return (status);
}
