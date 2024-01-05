/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_random.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cestevez <cestevez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/04 21:58:10 by cestevez          #+#    #+#             */
/*   Updated: 2024/01/04 21:58:23 by cestevez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	remove_quotes(t_shell *shell)
{
	int				i;
	int				current_len;
	t_simple_cmds	*current_cmd;
	char			*truncated;

	i = 0;
	truncated = NULL;
	current_cmd = shell->cmds;
	while (current_cmd != NULL) 
	{
		i = 0;
		while (current_cmd->str != NULL && current_cmd->str[i] != NULL)
		{
			current_len = ft_strlen(current_cmd->str[i]);
			if (current_cmd->str[i][0] == '\''
				&& current_cmd->str[i][current_len - 1] == '\'')
			{
				truncated = ft_substr(current_cmd->str[i], 1, current_len - 2);
				free(current_cmd->str[i]);
				current_cmd->str[i] = truncated;
			}
			i++;
		}
		current_cmd = current_cmd->next;
	}
}
