/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emollebr <emollebr@student.42berlin.d      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/13 21:44:54 by emollebr          #+#    #+#             */
/*   Updated: 2023/12/15 10:54:32 by emollebr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	get_expanded_exitcode(char **str, int start, int end)
{
	char	*var;
	char	*tmp;
	char	*ret;
	char	*append;
	int		new_index;
	int		exitcode;

	exitcode = g_last_exit;
	var = ft_itoa(exitcode);
	ret = ft_substr(*str, 0, start);
	tmp = ft_strjoin(ret, var);
	free(ret);
	free(var);
	new_index = ft_strlen(tmp);
	append = ft_substr(*str, end, ft_strlen(*str) - end + 1);
	free(*str);
	*str = ft_strjoin(tmp, append);
	if (!str)
		return (-1);
	free(tmp);
	free(append);
	return (new_index);
}

int	get_expanded_variable(char **str, int start, int end, t_shell *shell)
{
	char	*var;
	char	*tmp;
	char	*ret;
	char	*append;
	int		new_index;

	var = ft_substr(*str, start, end - start);
	tmp = ft_strtrim(var, "}${"); 
	free(var);
	var = ft_getenv(tmp, shell->env);
	if (!var)
		var = "";
	free(tmp);
	tmp = ft_strdup(var);
	ret = ft_substr(*str, 0, start);
	var = ft_strjoin(ret, tmp);
	free(ret);
	free(tmp);
	new_index = ft_strlen(var);
	append = ft_substr(*str, end, ft_strlen(*str) - end + 1);
	free(*str);
	*str = ft_strjoin(var, append);
	free(var);
	free(append);
	return (new_index);
}

int	get_quotes_trimmed(char **str, int start, int end)
{
	char	*prepend;
	char	*append;
	char	*tmp;
	int		new_index;

	prepend = ft_substr(*str, 0, start);
	append = ft_substr(*str, start + 1, end - start - 1);
	tmp = ft_strjoin(prepend, append);
	new_index = ft_strlen(tmp);
	free(prepend);
	free(append);
	append = ft_substr(*str, end + 1, ft_strlen(*str) - (end - start));
	free(*str);
	*str = ft_strjoin(tmp, append);
	free(append);
	free(tmp);
	return (new_index);
}

char	*check_for_variables(char *str, t_shell *shell)
{
	int	i;
	int	q_flag;

	i = 0;
	q_flag = -1;
	while (str && str[i] != '\0')
	{
		i = if_quotes(&str, &q_flag, i);
		if (i == -1)
			return (NULL);
		if (str[i] != '\0' && str[i] == '$' && str[i + 1] == '?')
			i = get_expanded_exitcode(&str, i, i + 2) - 1;
		else 
			i = if_variable(&str, shell, i);
		if (i == -1)
			return (NULL);
		else if (str[i] == '$' && str[i + 1] != '\0' 
			&& ft_strchr("\'\"", str[i + 1]) != 0)
			str = ft_strtrim(str, "$");
		if (str[i] != '\0')
			i++;
	}
	return (str);
}

int	expander(t_shell *shell)
{
	t_simple_cmds	*current;
	char			*tmp;
	int				i;

	current = shell->cmds;
	i = 0;
	while (current != NULL)
	{
		i = 0;
		while (current->str != NULL && current->str[i] != NULL)
		{
			current->str[i] = check_for_variables(current->str[i], shell);
			if (current->str[i] == NULL)
			{
				tmp = current->str[i];
				current->str[i] = current->str[i + 1];
				free(tmp);
			}
			i++;
		}
		current = current->next;
	}
	return (0);
}
