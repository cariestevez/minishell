﻿#include "minishell.h"

//add specific error returns with perror(errno)
//handle #(comment) when in middle of str?

// checks if token is an operator(pipe or redir)
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

//iterates the command line saving each command as an individual node,
//using operators and quotation/braces as delimiters
int    read_command_line(t_lexer *lexer, char *str)
{
	int	i;
	int	start;
	int	temp;
	int	flag;

	i = 0;
	start = 0;
	temp = 0;
	flag = 0;
	while (str[i] != '\0' && str[i] == ' ')
		i++;
	if (str[i] == '#')
		return (0);
	while (i < (int)ft_strlen(str))
	{
		start = i;
		if (str[i] == '\'' || str[i] == '"' || str[i] == '(' || str[i] == '{')
		{
			if (str[i] == '\'' || str[i] == '"')
				temp = str[i];
			else if (str[i] == ')')
				temp = ')';
			else
				temp = '}';
			i++;
			while (str[i] != temp)
				i++;
			if (!(save_token(lexer, str, start, i - start + 1)))
				return (0);
		}
		else if (str[i] == '>' || str[i] == '<' || str[i] == '|')
		{
			if ((str[i] == '>' && str[i + 1] == '>') || (str[i] == '<' && str[i + 1] == '<'))
				flag = 1;
			if (!(save_token(lexer, str, start, 1 + flag)))
				return (0);
		}
		else
		{
			while (str[i] && str[i] != '>' && str[i] != '<' && str[i] != '|' && str[i] != ' ')
				i++;
			if (!(save_token(lexer, str, start, i - start)))
				return (0);
			if (str[i] == ' ')
				i--;
		}
		i += flag + 1;
		if (i > (int)ft_strlen(str))
			break ;
		while (str[i] && str[i] == ' ')
			i++;
		lexer = lexer->next;
	}
	return (1);
}

t_lexer	*ft_lexer(char *str)
{
	t_lexer     *lexer;
	t_lexer     *head;

	lexer = NULL;
	head = NULL;
	if (open_brackets(str) || open_quotes(str) || open_curly(str))
	{
		ft_printf("syntax error near unexpected token");//print specific for each case?
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
