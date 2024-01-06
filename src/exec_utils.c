/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cestevez <cestevez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/13 21:39:39 by emollebr          #+#    #+#             */
/*   Updated: 2024/01/06 21:57:14 by cestevez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

int	close_unneccesary_fds(int **fd, int i, int amount_of_cmds)
{
	int	j;

	j = 0;
	if (!fd)
		return (1);
	while (j < amount_of_cmds)
	{
		if (i - 1 != j)
			close(fd[j][0]);
		if (i != j)
			close(fd[j][1]);
		j++;
	}
	return (0);
}

char	*get_path(char *cmd, char **env)
{
	int		i;
	char	*path;
	char	**fullpath;
	char	*path_elem;

	i = 0;
	if (ft_strlen(cmd) == 0)
		return (cmd);
	fullpath = ft_split(ft_getenv("PATH", env), ':');
	while (fullpath[i])
	{
		path_elem = ft_strjoin(fullpath[i], "/");
		path = ft_strjoin(path_elem, cmd);
		free(path_elem);
		if (access(path, F_OK | X_OK) == 0)
			return (path);
		free(path);
		i++;
	}
	free_char_arr(fullpath);
	return (cmd);
}

char	*ft_getenv(char *name, char **env)
{
	int		i;
	int		j;
	char	*sub;

	i = 0;
	while (env[i])
	{
		j = 0;
		while (env[i][j] && env[i][j] != '=')
			j++;
		sub = ft_substr(env[i], 0, j);
		if (ft_strncmp(sub, name, 100) == 0)
		{
			free(sub);
			return (env[i] + j + 1);
		}
		free(sub);
		i++;
	}
	return (NULL);
}

int	free_and_exit(t_shell *shell, int **fd, pid_t *pid)
{
	int	aoc;

	aoc = shell->amount_of_cmds;
	if (fd != NULL)
	{
		close_unneccesary_fds(fd, aoc + 1, aoc);
		free_int_arr(fd);
	}
	free(pid);
	return (errno);
}
