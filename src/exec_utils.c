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
	int j;

	j = 0;
	if (!fd)
		return (1);
	while (j < amount_of_cmds)
	{
		if (i - 1 != j)
			close (fd[j][0]);
		if (i != j)
			close (fd[j][1]);
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
	char	**s_cmd;

	i = 0;
	fullpath = ft_split(ft_getenv("PATH", env), ':');
	s_cmd = ft_split(cmd, ' ');
	while (fullpath[i])
	{
		path_elem = ft_strjoin(fullpath[i], "/");
		path = ft_strjoin(path_elem, s_cmd[0]);
		free(path_elem);
		if (access(path, F_OK | X_OK) == 0)
		{
			free_char_arr(s_cmd);
			return (path);
		}
		free(path);
		i++;
	}
	free_char_arr(fullpath);
	free_char_arr(s_cmd);
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

int    free_and_exit(t_shell *shell, int **fd, int exitcode)
{
    //this function call will close ALL fds
	if (fd != NULL)
	{
		close_unneccesary_fds(fd, shell->amount_of_cmds + 1, shell->amount_of_cmds);
	 	free_int_arr(fd); //when commenting it out, we don't get the free() invalid pointer anymore
	}
	return (exitcode);
}