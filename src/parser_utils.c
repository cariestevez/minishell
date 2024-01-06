/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cestevez <cestevez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/15 10:49:08 by emollebr          #+#    #+#             */
/*   Updated: 2024/01/06 22:44:08 by cestevez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	count_tokens(t_lexer *lexer)
{
	int	tokens_and_redirs[0];
	int	redir_tokens;

	tokens_and_redirs[0] = 0;
	redir_tokens = 0;
	while (lexer != NULL && lexer->token != NULL && lexer->key != l_pipe)
	{
		if (lexer->key == l_in || lexer->key == l_out
			|| lexer->key == l_append || lexer->key == l_heredoc)
			redir_tokens++;
		tokens_and_redirs[0]++;
		lexer = lexer->next;
	}
	tokens_and_redirs[0] -= (redir_tokens * 2);
	if (tokens_and_redirs[0] <= 0 && redir_tokens != 0)
		return (0);
	if (tokens_and_redirs[0] <= 0)
		return (-1);
	return (tokens_and_redirs[0]);
}

int	count_cmds(t_lexer	*lexer)
{
	int	cmds;

	cmds = 1;
	while (lexer != NULL)
	{
		if (lexer->key == l_pipe)
		{
			if (lexer->prev == NULL || lexer->next == NULL)
			{
				ft_printf("syntax error near unexpected token `|'\n");
				return (0);
			}
			cmds++;
		}
		if ((lexer->key == l_in || lexer->key == l_out
				|| lexer->key == l_append || lexer->key == l_heredoc)
			&& lexer->next == NULL)
		{
			ft_printf("syntax error near unexpected token `newline'\n");
			return (0);
		}
		lexer = lexer->next;
	}
	return (cmds);
}

t_simple_cmds	*new_cmd_node(t_simple_cmds *prev)
{
	t_simple_cmds	*node;

	node = (t_simple_cmds *)malloc(sizeof(t_simple_cmds));
	if (node == NULL)
		return (NULL);
	node->str = NULL;
	node->builtin = NULL;
	node->redir = NULL;
	node->prev = prev;
	node->next = NULL;
	if (!prev)
	{
		node->index = 0;
		node->redir_head = NULL;
	}
	else
	{
		node->redir_head = prev->redir_head;
		node->index = prev->index + 1;
	}
	return (node);
}

t_redir	*new_redir_node(char *file, t_lexertype type)
{
	t_redir	*node;

	node = (t_redir *)ft_calloc(sizeof(t_redir), 1);
	if (!node)
		return (NULL);
	node->type = type;
	node->str = ft_strdup(file);
	if (!node->str)
		return (NULL);
	node->next = NULL;
	return (node);
}

void	add_builtin_ptr(t_simple_cmds *cmd)
{
	t_simple_cmds	*head;

	head = cmd;
	while (cmd != NULL && cmd->str != NULL && cmd->str[0] != NULL)
	{
		if (ft_strncmp(cmd->str[0], "cd", BUFFER) == 0)
			cmd->builtin = &ft_cd;
		else if (ft_strncmp(cmd->str[0], "echo", BUFFER) == 0)
			cmd->builtin = &ft_echo;
		else if (ft_strncmp(cmd->str[0], "env", BUFFER) == 0)
			cmd->builtin = &ft_env;
		else if (ft_strncmp(cmd->str[0], "export", BUFFER) == 0)
			cmd->builtin = &ft_export;
		else if (ft_strncmp(cmd->str[0], "pwd", BUFFER) == 0)
			cmd->builtin = &ft_pwd;
		else if (ft_strncmp(cmd->str[0], "unset", BUFFER) == 0)
			cmd->builtin = &ft_unset;
		else if (ft_strncmp(cmd->str[0], "exit", BUFFER) == 0)
			cmd->builtin = &ft_exit;
		cmd = cmd->next;
	}
	cmd = head;
	return ;
}
