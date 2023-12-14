/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emollebr <emollebr@student.42berlin.d      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/14 14:08:07 by emollebr          #+#    #+#             */
/*   Updated: 2023/12/14 14:08:09 by emollebr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "minishell.h"

t_lexertype get_key(char *str)
{
	int	i;

	i = 0;
	if (str[i] == '<')
	{
		if (str[i + 1] == '<')
			return (l_heredoc);
		else
			return (l_in);
	}
	if (str[i] == '>')
	{
		if (str[i + 1] == '>')
			return (l_append);
		else
			return (l_out);
	}
	if (str[i] == '|')
		return (l_pipe);
	return (l_non_op);
}

// saves the token and its key in the node
int	save_token(t_lexer *lexer, char *str, int start, int len)
{
	//ft_printf("token len: %d\n", len);
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

int	check_for_quotes(char *str, t_lexer *lexer, int start)
{
	char	temp;
	int			i;

	i = start;
	temp = '\0';
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

int	check_for_redirections(char *str, t_lexer *lexer, int start)
{
	char	flag;
	int			i;

	i = start;
	flag = 0;
	if ((str[i] == '>' && str[i + 1] == '>') || (str[i] == '<' && str[i + 1] == '<'))
		flag = 1;
	i += 1 + flag;
	if (!(save_token(lexer, str, start, 1 + flag)))
			return (-1);
	return (i);
}

int    read_command_line(t_lexer *lexer, char *str, int i)
{
	if (str[i] == '#')
		return (0);
	while (i < (int)ft_strlen(str) && i != -1)
	{
		while (str[i] != '\0' && str[i] == ' ')
			i++;
		while (str[i] != '\0')
		{
			if (str[i] == '>' || str[i] == '<' || str[i] == '|')
				i = check_for_redirections(str, lexer, i);
			else
				i = check_for_quotes(str, lexer, i);
			while (str[i] != '\0' && str[i] == ' ' && i != -1)
				i++;
			lexer = lexer->next;
		}
	}
	return (i);
}

t_lexer	*ft_lexer(char *str)
{
	t_lexer     *lexer;
	t_lexer     *head;
	int				i;

	lexer = NULL;
	head = NULL;
	i = 0;
	if (open_brackets(str) || open_quotes(str) || open_curly(str))
		return (ft_printf("syntax error near unexpected token"), NULL);
	lexer = new_lexnode(NULL, 0);
	if (lexer == NULL)
		return (NULL);
	head = lexer;
	while (str[i] != '\0' && str[i] == ' ')
		i++;
	if (!read_command_line(lexer, str, i))
	{
		free_lexer(head);
		return (NULL);
	}
	return (head);
}
