/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emollebr <emollebr@student.42berlin.d      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/15 13:36:24 by emollebr          #+#    #+#             */
/*   Updated: 2023/12/15 13:36:25 by emollebr         ###   ########.fr       */
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
		if (lexer->key == l_in || lexer->key == l_out || lexer->key == l_append || lexer->key == l_heredoc)
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

t_redir	*save_redirection(t_shell *shell, t_lexer *lexer, int redir_count)
{
	if (lexer->next == NULL)
	{
		ft_printf("syntax error near unexpected token 'newline'");
		return (NULL);
	}
	if (redir_count == 1)
	{
		shell->cmds->redir = new_redir_node(lexer->next->token, lexer->key);
		if (shell->cmds->redir == NULL)
			return (NULL);
		shell->cmds->redir_head = shell->cmds->redir;
	}
	else
	{
		shell->cmds->redir->next = new_redir_node(lexer->next->token, lexer->key);
		if (shell->cmds->redir == NULL)
			return (NULL);
	}
	shell->cmds->redir = shell->cmds->redir->next;
	return (shell->cmds->redir);
}

t_lexer	*save_cmd_str(t_shell *shell, t_lexer *lexer, int *tokens_and_redirs, int i)
{
	while (lexer != NULL && lexer->token != NULL && lexer->key != l_pipe)
	{
		if (lexer->key == l_in || lexer->key == l_out
			|| lexer->key == l_append || lexer->key == l_heredoc)
		{
			tokens_and_redirs[1]++;
			shell->cmds->redir = save_redirection(shell, lexer, tokens_and_redirs[1]);
			if (shell->cmds->redir == NULL)
				return (NULL);
			lexer = lexer->next->next;
		}
		else if (lexer->key == l_non_op)
		{
			while (tokens_and_redirs[0] > 0 && lexer && lexer->key == l_non_op)
			{
				shell->cmds->str[i] = ft_strdup(lexer->token);
				i++;
				tokens_and_redirs[0]--;
				lexer = lexer->next;
			}
			shell->cmds->str[i] = NULL;
		}
	}
	return (lexer);
}

int	save_simple_cmd(t_lexer	*lexer, t_shell	*shell)
{
	int		tokens_and_redirs[2];
	int		i;
	t_lexer	*lexer_copy;

	i = 0;
	lexer_copy = lexer;
	tokens_and_redirs[0] = count_tokens(lexer_copy);
	tokens_and_redirs[1] = 0;
	if (tokens_and_redirs[0] < 0)
		return (-1);
	if (tokens_and_redirs[0] != 0)
	{
		shell->cmds->str = ft_calloc(sizeof(char *), (tokens_and_redirs[0] + 1));
		if (shell->cmds->str == NULL)
			return (-1);
	}
	lexer = save_cmd_str(shell, lexer, tokens_and_redirs, i);
	if (lexer && lexer->key == l_pipe)
		lexer = lexer->next;
	if (lexer == NULL)
		return (-1);
	return (lexer->index);
}

int	parser_loop(t_shell *shell, t_lexer *lexer)
{
	int	cmd;
	int	idx;

	idx = 0;
	cmd = 0;
	while (cmd < shell->amount_of_cmds)
	{
		idx = save_simple_cmd(lexer, shell);
		while (idx > 0 && lexer && lexer->index != idx)
			lexer = lexer->next;
		if (lexer == NULL)
			return (free_cmds(shell->cmd_head), -1);
		if (cmd < shell->amount_of_cmds - 1)
		{
			shell->cmds->next = new_cmd_node(shell->cmds);
			if (shell->cmds->next == NULL)
				return (free_cmds(shell->cmd_head), -1);
			shell->cmds = shell->cmds->next;
		}
		cmd++;
	}
	return (0);
}

t_simple_cmds	*ft_parser(t_lexer *lexer, t_shell *shell)
{
	shell->amount_of_cmds = count_cmds(lexer);
	if (shell->amount_of_cmds == 0)
		return (NULL);
	shell->cmds = new_cmd_node(NULL);
	if (shell->cmds == NULL)
		return (NULL);
	if (parser_loop(shell, lexer) != 0)
		return (NULL);
	while (shell->cmds->index != 0)
		shell->cmds = shell->cmds->prev;
	add_builtin_ptr(shell->cmds);
	return (shell->cmds);
}

void	add_builtin_ptr(t_simple_cmds *cmd)
{
	t_simple_cmds *head;

	head = cmd;
	while (cmd != NULL && cmd->str != NULL && cmd->str[0] != NULL)
	{
		if (cmd->str != NULL && ft_strncmp(cmd->str[0], "cd", BUFFER) == 0)
			cmd->builtin = &ft_cd;
		else if (cmd->str != NULL && ft_strncmp(cmd->str[0], "echo", BUFFER) == 0)
			cmd->builtin = &ft_echo;
		else if (cmd->str != NULL && ft_strncmp(cmd->str[0], "env", BUFFER) == 0)
			cmd->builtin = &ft_env;
		else if (cmd->str != NULL && ft_strncmp(cmd->str[0], "export", BUFFER) == 0)
			cmd->builtin = &ft_export;
		else if (cmd->str != NULL && ft_strncmp(cmd->str[0], "pwd", BUFFER) == 0)
			cmd->builtin = &ft_pwd;
		else if (cmd->str != NULL && ft_strncmp(cmd->str[0], "unset", BUFFER) == 0)
			cmd->builtin = &ft_unset;
		else if (cmd->str != NULL && ft_strncmp(cmd->str[0], "exit", BUFFER) == 0)
			cmd->builtin = &ft_exit;
		cmd = cmd->next;
	}
	cmd = head;
	return ;
}
