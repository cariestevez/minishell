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

void    free_array(int  **arr)
{
    int i;

    i = 0;
    while (arr[i])
    {
        free(arr[i]);
        i++;
    }
    free(arr);
}

void    free_simple_cmds(t_simple_cmds *cmds)
{
    int i;

    i = 0;
    while (cmds)
    {
        free_tab(cmds->str);
        free(cmds->in);
        free(cmds->out);
        if (cmds->index > 0)
            free(cmds->prev);
        if (cmds->next == NULL)
        {
            free(cmds);
            return ;
        }
        cmds = cmds->next;
    }
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

int	close_unneccesary_fds(int **fd, int i, int amount_of_cmds)
{
	int j;

	j = 0;
	while (j < amount_of_cmds)
	{
		if (i - 1 != j)
			close (fd[j][0]);
		if (i != j)
			close (fd[j][1]);
		j++;
	}
	return (1);
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