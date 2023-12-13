/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_unset.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emollebr <emollebr@student.42berlin.d      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/13 20:23:15 by emollebr          #+#    #+#             */
/*   Updated: 2023/12/13 20:23:17 by emollebr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_unset(t_shell *shell, t_simple_cmds *cmd)
{
	char	**list;
	int		i;
	int		j;

	list = shell->env;
	i = 1;
	j = 0;
	while (cmd->str[i])
	{
		j = 0;
		while (ft_strncmp(cmd->str[i], list[j], ft_strlen(cmd->str[i])) != 0
			&& list[j] != NULL)
		{
			j++;
		}
		if (cmd->str[i] != NULL)
		{
			free(list[j]);
			while (list[j] != NULL)
			{
				list[j] = list[j + 1];
				j++;
			}
			free(list[j]);
		}
		i++;
	}
	return (0);
}
