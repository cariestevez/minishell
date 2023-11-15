#include "../minishell.h"

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
			free_tab(s_cmd);
			return (path);
		}
		free(path);
		i++;
	}
	free_tab(fullpath);
	free_tab(s_cmd);
	return (cmd);
}

char    *get_curpath(char *directory, char **env)
{
    char    *curpath;
    
    if (ft_strncmp(directory, "-", 2) == 0) //man cd (OPERANDS)
        return (ft_getenv("OLDPWD", env));
    if (!directory )
    {
        if (!ft_getenv("HOME", env)) //man cd (1.)
            return (NULL);
        directory = ft_strdup(ft_getenv("HOME", env)); //man cd (2.)
    }
    if (ft_strncmp(directory, "~", 1) == 0)
    {
        curpath = ft_strjoin(ft_getenv("HOME", env), "/");
        directory = ft_strjoin(curpath, directory + 1);
        free(curpath);
    }
    if (directory[0] == '/' || directory[0] == '.')  //man cd (3+4.)
        curpath = directory;
    else
        curpath = get_path(directory, env); //man cd (5.)
    return (curpath);
}

int    update_envvar(char *name, char *new, char **env)
{
    int i;

    i = 0;
    while (ft_strncmp(env[i], name, ft_strlen(name)) != 0)
        i++;
    free(env[i]);
    name = ft_strjoin(name, "=");
    env[i] = ft_strjoin(name, new);
    free(new);
    if (!env[i])
        return (-1);
    return (0);
}

int ft_cd(t_shell *shell)
{
    char    *curpath;
    char    *old_pwd;

    ft_printf("the str is %s\n\n", shell->cmds->str[0]);
    curpath = get_curpath(shell->cmds->str[0], shell->env);

    ft_printf("curpath is %s after get_curpath\n", curpath);
    //save old pwd to update envvar OLD_PWD
    old_pwd = getcwd(NULL, 0);
    //alter OLD_PWD in env 
    if (update_envvar("OLDPWD", old_pwd, shell->env) != 0)
        return (-1);
    //changes directory
    chdir(curpath);
    //free(curpath);
    curpath = getcwd(NULL, 0);
    //alter PWD in env
    if (update_envvar("PWD", curpath, shell->env) != 0)
        return (-1);
    if (ft_strncmp(shell->cmds->str[0], "-", 2) == 0) //man cd (OPERANDS)
        ft_pwd();
    return (0);
}

char    **arrdup(char **env)
{
    int     i;
    char    **ret;
    i = 0;
    while (env[i])
        i++;
    ret = ft_calloc(sizeof(char *), i + 2);
    if (!ret)
        return (NULL);
    i = 0;
    while (env[i] != NULL)
    {
        ret[i] = ft_strdup(env[i]);
        if (ret[i] == NULL)
            return (NULL);
        i++;
    }
    ret[i] = NULL;
    return (ret);
}

int main(int ac, char **av, char **envp)
{
    t_shell         *shell;
    char            *str[2];
    int                 i;

    (void)ac;
    (void)av;
 
    shell = malloc(sizeof(t_shell *));
    shell->cmds = malloc(sizeof(t_simple_cmds *));
    shell->env = arrdup(envp);
    ft_printf("env before cd:\n");
    i = 0;
    while (shell->env[i])
    {
        ft_printf("%s\n", shell->env[i]);
        i++;
    }
    str[0] = ft_strdup("~Desktop/");
    str[1] = ((void*)0);
    shell->cmds->str = str;
    shell->cmds->builtin = &ft_cd;
    shell->cmds->builtin(shell);
    ft_printf("envcpy after ft_cd:\n");
    i = 0;
    while (shell->env[i])
    {
        ft_printf("%s\n", shell->env[i]);
        free(shell->env[i]);
        i++;
    }
    free(shell->env);
    free(shell->cmds->str[0]);
    free(shell->cmds);
    free(shell);
}
