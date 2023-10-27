#include "parser.h"
/*
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
			ft_free_tab(s_cmd);
			return (exec);
		}
		free(exec);
		i++;
	}
	ft_free_tab(fullpath);
	ft_free_tab(s_cmd);
	return (cmd);
}

void	execute(char *cmd, char **env)
{
	char	**s_cmd;
	char	*path;	

	s_cmd = ft_split(cmd, ' ');
	write(2, "\n", 1);
	path = get_path(s_cmd[0]);
	if (execve(path, s_cmd, env) == -1)
	{
		ft_putstr_fd("minishell: command not found: ", 2);
		ft_putendl_fd(s_cmd[0], 2);
		ft_free_tab(s_cmd);
		exit(127);
	}
}*/

int executer(t_simple_cmds *cmds)
{
    expander(cmds);
    /*if (!cmds->next)
    {
        single_cmd();
    }*/
    //heredoc();
    //execute
    return (0);
}