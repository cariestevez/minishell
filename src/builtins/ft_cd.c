/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emollebr <emollebr@student.42berlin.d      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/13 20:22:38 by emollebr          #+#    #+#             */
/*   Updated: 2023/12/13 20:22:40 by emollebr         ###   ########.fr       */
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
		printf("minishell: cd: %s: No such file or directory\n", cmd->str[1]);
		return (2);
	}
	old_pwd = getcwd(NULL, 0);
	update_envvar("OLDPWD", old_pwd, shell->env);
	if (chdir(curpath) < 0)
		return (perror("minishell: cd"), 2);
	curpath = getcwd(NULL, 0);
	update_envvar("PWD", curpath, shell->env);
	return (0);
}
