/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cestevez <cestevez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/13 20:22:38 by emollebr          #+#    #+#             */
/*   Updated: 2024/01/06 22:50:10 by cestevez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*get_curpath(char *directory, char **env)
{
	char	*curpath;

	if (!directory)
		return (ft_strdup(ft_getenv("HOME", env)));
	if (ft_strncmp(directory, "-", 2) == 0)
	{
		ft_printf("%s\n", ft_getenv("OLDPWD", env));
		return (ft_strdup(ft_getenv("OLDPWD", env)));
	}
	if (ft_strncmp(directory, "~", 1) == 0)
	{
		curpath = ft_strjoin(ft_getenv("HOME", env), "/");
		directory = ft_strjoin(curpath, directory + 1);
		free(curpath);
	}
	if (directory[0] == '/' || directory[0] == '.')
		curpath = directory;
	else
		curpath = get_path(directory, env);
	return (curpath);
}

int	update_envvar(char *name, char *new, char **env)
{
	int	i;

	i = 0;
	while (ft_strncmp(env[i], name, ft_strlen(name)) != 0)
		i++;
	free(env[i]);
	name = ft_strjoin(name, "=");
	env[i] = ft_strjoin(name, new);
	free(new);
	free(name);
	return (0);
}

int	ft_cd(t_shell *shell, t_simple_cmds *cmd)
{
	char	*curpath;
	char	*old_pwd;

	curpath = get_curpath(cmd->str[1], shell->env);
	if (curpath == NULL)
	{
		ft_printf("minishell: cd: %s: ", cmd->str[1]);
		ft_printf("No such file or directory\n");
		return (2);
	}
	old_pwd = getcwd(NULL, 0);
	update_envvar("OLDPWD", old_pwd, shell->env);
	if (chdir(curpath) < 0)
		return (perror("minishell: cd"), 2);
	if (cmd->str[1] == NULL)
		free(curpath);
	curpath = getcwd(NULL, 0);
	update_envvar("PWD", curpath, shell->env);
	return (0);
}
