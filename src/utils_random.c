/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_random.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cestevez <cestevez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/04 21:58:10 by cestevez          #+#    #+#             */
/*   Updated: 2024/01/06 17:12:22 by cestevez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*util_remove_quotes(t_simple_cmds *current_cmd, int i)
{
	char			*truncated;
	int				current_len;

	truncated = NULL;
	current_len = ft_strlen(current_cmd->str[i]);
	if ((current_cmd->str[i][0] == '\''
		|| current_cmd->str[i][0] == '\"')
		&& (current_cmd->str[i][current_len - 1] == '\''
		|| current_cmd->str[i][current_len - 1] == '\"'))
	{
		truncated = ft_substr(current_cmd->str[i], 1, current_len - 2);
		free(current_cmd->str[i]);
		current_cmd->str[i] = truncated;
	}
	return (current_cmd->str[i]);
}

void	remove_quotes(t_shell *shell)
{
	int				i;
	t_simple_cmds	*current_cmd;

	i = 0;
	current_cmd = shell->cmds;
	while (current_cmd != NULL)
	{
		i = 0;
		while (current_cmd->str != NULL && current_cmd->str[i] != NULL)
		{
			current_cmd->str[i] = util_remove_quotes(current_cmd, i);
			i++;
		}
		current_cmd = current_cmd->next;
	}
}

void	trim_dollar_sign(char **str)
{
	char	*tmp;

	tmp = ft_strtrim(*str, "$");
	free(*str);
	*str = tmp;
}

int	restore_streams(int *std)
{
	if (dup2(std[0], 0) < 0)
		return (perror("dup2"), 1);
	close(std[0]);
	if (dup2(std[1], 1) < 0)
		return (perror("dup2"), 1);
	close(std[1]);
	return (0);
}

int	check_edgecases(t_shell *shell)
{
	if (shell->amount_of_cmds == 1 && shell->cmds->builtin != NULL)
	{
		execute_builtin(shell, NULL, 0);
		restore_streams(shell->std);
		return (1);
	}
	if (shell->cmds->redir && !shell->cmds->str)
	{
		redirections(shell->cmds);
		restore_streams(shell->std);
		return (1);
	}
	return (0);
}
