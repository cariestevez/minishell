/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cestevez <cestevez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/15 14:38:38 by emollebr          #+#    #+#             */
/*   Updated: 2024/01/05 10:59:52 by cestevez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_char_arr(char **str)
{
	int	i;

	i = 0;
	if (str != NULL)
	{
		while (str[i] != NULL)
		{
			free(str[i]);
			str[i] = NULL;
			i++;
		}
		free(str);
		str = NULL;
	}
}

void	free_int_arr(int	**arr)
{
	int	i;

	i = 0;
	if (arr != NULL)
	{
		while (arr[i] != NULL)
		{
			free(arr[i]);
			arr[i] = NULL;
			i++;
		}
		free(arr);
		arr = NULL;
	}
}

int	free_lexer(t_lexer *lexer)
{
	t_lexer	*tmp;

	while (lexer != NULL)
	{
		tmp = lexer->next;
		free(lexer->token);
		lexer->token = NULL;
		free(lexer);
		lexer = tmp;
	}
	return (0);
}

void	free_cmds(t_simple_cmds *cmd_node)
{
	t_simple_cmds	*temp;
	t_redir			*redir_tmp;

	temp = NULL;
	redir_tmp = NULL;
	if (cmd_node == NULL)
		return ;
	cmd_node->redir = cmd_node->redir_head;
	while (cmd_node != NULL)
	{
		temp = cmd_node->next;
		while (cmd_node->redir != NULL)
		{
			free(cmd_node->redir->str);
			cmd_node->redir->str = NULL;
			redir_tmp = cmd_node->redir->next;
			free(cmd_node->redir);
			cmd_node->redir = redir_tmp;
		}
		free_char_arr(cmd_node->str);
		cmd_node->str = NULL;
		free(cmd_node);
		cmd_node = temp;
	}
}

void	free_on_succes(t_simple_cmds *cmds, t_lexer *lexer, char *prompt)
{
	free_lexer(lexer);
	free(prompt);
	free_cmds(cmds);
	cmds = NULL;
}
