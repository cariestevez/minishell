/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emollebr <emollebr@student.42berlin.d      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/05 09:39:26 by emollebr          #+#    #+#             */
/*   Updated: 2024/01/05 09:39:27 by emollebr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "minishell.h"

int	save_token(t_lexer *lexer, char *str, int start, int len)
{
	lexer->token = ft_substr(str, start, len);
	if (!lexer->token)
		return (0);
	lexer->key = get_key(lexer->token);
	if (start + len < (int)ft_strlen(str))
	{
		lexer->next = new_lexnode(lexer, lexer->index + 1);
		if (!lexer->next)
			return (0);
	}
	lexer = lexer->next;
	return (1);
}

int	tokenize_redir(t_lexer *lexer, char *str, int start, int i)
{
	int	flag;

	flag = 0;
	if ((str[i] == '>' && str[i + 1] == '>')
		|| (str[i] == '<' && str[i + 1] == '<'))
		flag = 1;
	i += 1 + flag;
	if (!(save_token(lexer, str, start, 1 + flag)))
		return (-1);
	return (i);
}

int	tokenize_cmd(t_lexer *lexer, char *str, int start, int i)
{
	int	temp;

	temp = 0;
	if (str[i] == '\'' || str[i] == '\"')
	{
		temp = str[i];
		i++;
		while (str[i] != temp)
			i++;
	}
	while (str[i] != '\0' && str[i] != ' ' && str[i] != '>'
		&& str[i] != '<' && str[i] != '|')
		i++;
	if (!(save_token(lexer, str, start, i - start)))
		return (-1);
	return (i);
}

int	read_command_line(t_lexer *lexer, char *str)
{
	int	i;
	int	start;

	i = 0;
	start = 0;
	while (i < (int)ft_strlen(str))
	{
		i = skip_spaces(str, i);
		while (str[i] != '\0')
		{
			start = i;
			if (str[i] == '>' || str[i] == '<' || str[i] == '|')
				i = tokenize_redir(lexer, str, start, i);
			else
				i = tokenize_cmd(lexer, str, start, i);
			if (i < 0)
				return (0);
			i = skip_spaces(str, i);
			lexer = lexer->next;
		}
	}
	return (1);
}

t_lexer	*ft_lexer(char *str)
{
	t_lexer	*lexer;
	t_lexer	*head;

	lexer = NULL;
	head = NULL;
	if (open_brackets(str) || open_quotes(str) || open_curly(str))
	{
		ft_printf("syntax error near unexpected token\n");
		return (NULL);
	}
	lexer = new_lexnode(NULL, 0);
	if (lexer == NULL)
		return (NULL);
	head = lexer;
	if (!read_command_line(lexer, str))
	{
		free_lexer(head);
		return (NULL);
	}
	return (head);
}
