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

t_redir	*save_redirection(t_shell *shell, t_lexer *lexer, int redir_count)
{
	t_redir	*current_redir;

	current_redir = shell->cmds->redir;
	if (lexer->next == NULL)
	{
		ft_printf("syntax error near unexpected token 'newline'");
		return (NULL);
	}
	if (redir_count == 1)
	{
		current_redir = new_redir_node(lexer->next->token, lexer->key);
		if (current_redir == NULL)
			return (NULL);
		shell->cmds->redir_head = current_redir;
	}
	else
	{
		current_redir->next = new_redir_node(lexer->next->token, lexer->key);
		if (shell->cmds->redir == NULL)
			return (NULL);
	}
	current_redir = current_redir->next;
	return (current_redir);
}

t_lexer	*save_cmd_str(t_shell *shell, t_lexer *lexer, int *t_and_r, int i)
{
	while (lexer != NULL && lexer->token != NULL && lexer->key != l_pipe)
	{
		if (lexer->key == l_in || lexer->key == l_out
			|| lexer->key == l_append || lexer->key == l_heredoc)
		{
			t_and_r[1]++;
			shell->cmds->redir = save_redirection(shell, lexer, t_and_r[1]);
			if (shell->cmds->redir == NULL)
				return (NULL);
			lexer = lexer->next->next;
		}
		else if (lexer->key == l_non_op)
		{
			while (t_and_r[0] > 0 && lexer && lexer->key == l_non_op)
			{
				shell->cmds->str[i] = ft_strdup(lexer->token);
				i++;
				t_and_r[0]--;
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
		shell->cmds->str = ft_calloc(8, (tokens_and_redirs[0] + 1));
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
	expander(shell);
	return (shell->cmds);
}
