/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cestevez <cestevez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/13 21:42:05 by emollebr          #+#    #+#             */
/*   Updated: 2024/01/07 00:18:21 by cestevez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	replace_variable(char *var, char *new_var, t_shell *shell, int i)
{
	char	**current_env;

	current_env = ft_split(shell->env[i], '=');
	if (ft_strncmp(current_env[0], new_var, ft_strlen(new_var)) == 0)
	{
		free_char_arr(current_env);
		free(shell->env[i]);
		shell->env[i] = ft_strdup(var);
		return (0);
	}
	free_char_arr(current_env);
	return (1);
}

int	add_variable(char *var, t_shell *shell, int i)
{
	char	**new_env;

	new_env = ft_calloc(sizeof(char *), i + 2);
	if (!new_env)
		return (-1);
	new_env[i + 1] = NULL;
	new_env[i] = ft_strdup(var);
	while (--i >= 0)
		new_env[i] = ft_strdup(shell->env[i]);
	free_char_arr(shell->env);
	shell->env = new_env;
	return (0);
}

int	declare_variable(char *var, t_shell *shell)
{
	int		i;
	char	**new_var;

	i = 0;
	if (ft_strrchr(var, '=') == 0)
		return (-1);
	check_for_variables(var, shell);
	new_var = ft_split(var, '=');
	if (new_var == NULL)
		return (-1);
	while (shell->env[i] != NULL)
	{
		if (ft_strncmp(shell->env[i], var, ft_strlen(var)) == 0)
			return (free_char_arr(new_var), 1);
		if (replace_variable(var, new_var[0], shell, i) == 0)
			return (free_char_arr(new_var), 0);
		i++;
	}
	free_char_arr(new_var);
	add_variable(var, shell, i);
	return (0);
}

int	if_quotes(char **str, int *q_flag, int i)
{
	if ((*str)[i] == '\"')
	{
		ft_printf("in IF QUOTES\n");
		if (*q_flag != -1)
		{
			i = get_quotes_trimmed(str, *q_flag, i);
			if (!(*str))
				return (-1);
		}
		if (*q_flag == -1)
			*q_flag = i;
		else
			*q_flag = -1;
	}
	if ((*str)[i] == '\'' && *q_flag == -1)
	{
		i++;
		while ((*str)[i] != '\0' && (*str)[i] != '\'')
			i++;
		i--;
	}
	return (i);
}

int	if_variable(char **str, t_shell *shell, int i)
{
	int	start;

	start = 0;
	if ((*str)[i] == '$' && (*str)[i + 1] != '\0'
			&& (ft_isalnum((*str)[i + 1]) == 1
				|| ft_strchr("_{}", (*str)[i + 1]) != 0))
	{
		start = i;
		i++;
		while ((*str)[i] != '\0' && (ft_isalnum((*str)[i]) == 1
				|| ft_strchr("_{}", (*str)[i]) != 0))
			i++;
		i = get_expanded_variable(&(*str), start, i, shell) - 1;
	}
	ft_printf("str[%d]: %c\n", i, (*str)[i]);
	return (i);
}
