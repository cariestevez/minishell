#include "minishell.h"

//exit status 0 on success
//exit status 2 on incorrect usage, generally invalid options or missing arguments

char    *get_curpath(char *directory, char **env)
{
    char    *curpath;
    
    if (ft_strncmp(directory, "-", 2) == 0) //man cd (OPERANDS)
        return (ft_getenv("OLDPWD", env));
    if (!directory )
    {
        directory = ft_strdup(ft_getenv("HOME", env)); //man cd (2.)
        if (!directory) //man cd (1.)
            return (NULL);
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

int ft_cd(t_shell *shell, t_simple_cmds *cmd)
{
    char    *curpath;
    char    *old_pwd;

    if (!shell || !cmd)
        return (2);
    curpath = get_curpath(cmd->str[1], shell->env);
    if (!curpath)
        return (2);
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
    if (ft_strncmp(cmd->str[0], "-", 2) == 0) //man cd (OPERANDS)
        ft_pwd(shell, cmd);
    return (0);
}