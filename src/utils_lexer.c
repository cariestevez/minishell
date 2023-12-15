/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_lexer.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emollebr <emollebr@student.42berlin.d      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/15 10:38:20 by emollebr          #+#    #+#             */
/*   Updated: 2023/12/15 10:38:21 by emollebr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_lexer	*new_lexnode(t_lexer *prev, int index)
{
	t_lexer	*node;

	node = (t_lexer *)malloc(sizeof(t_lexer));
	if (node == NULL)
		return (NULL);
	node->token = NULL;
	node->key = 0;
	node->prev = prev;
	node->next = NULL;
	node->index = index;
	return (node);
}

int	open_quotes(char *str)
{
	int	i;
	int	tmp;

	i = 0;
	tmp = 0;
	while (str[i] != '\0')
	{
		if (str[i] == 34 || str[i] == 39)
		{
			tmp = str[i];
			i++;
			while (str[i] != tmp && str[i] != '\0')
				i++;
			if (str[i] == tmp)
				return (0);
			else if (str[i] == '\0')
				return (1);
		}
		i++;
	}
	return (0);
}

int	look_for(int bracket, char *str, int i)
{
	i++;
	while (str[i] != '\0' && str[i] != bracket)
		i++;
	return (i);
}

int	open_brackets(char *str)
{
	int	i;
	int	bracket;

	i = 0;
	bracket = 0;
	while (str[i] != '\0')
	{
		if (str[i] == 40 || str[i] == 41)
		{
			if (str[i] == 40)
				bracket = 41;
			else
				bracket = 40;
			i = look_for(bracket, str, i);
			if (str[i] == bracket && bracket == 41)
				return (0);
			if ((str[i] == bracket && bracket == 40) 
				|| (str[i] == '\0' && bracket == 41))
				return (41);
			else if (str[i] == '\0' && bracket == 40)
				return (40);
		}
		i++;
	}
	return (0);
}

int	open_curly(char *str)
{
	int	i;
	int	curly;

	i = 0;
	curly = 0;
	while (str[i] != '\0')
	{
		if (str[i] == 123 || str[i] == 125)
		{
			if (str[i] == 123)
				curly = 125;
			else
				curly = 123;
			i = look_for(curly, str, i);
			if (str[i] == curly && curly == 125)
				return (0);
			else if ((str[i] == curly && curly == 123) 
				|| (str[i] == '\0' && curly == 125))
				return (125);
			else if (str[i] == '\0' && curly == 123)
				return (123);
		}
		i++;
	}
	return (0);
}
