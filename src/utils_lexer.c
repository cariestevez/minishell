#include "minishell.h"

// initializes the new nodes of the lexer list
t_lexer *new_lexnode(t_lexer *prev, int index)
{
	t_lexer *node;
	
	ft_printf("---> new_lexnode\n");
	node = (t_lexer *)malloc(sizeof(t_lexer));
	if (node == NULL)
		return (NULL);
	ft_printf("allocated new lexer node\n");
	node->token = NULL;
	node->key = 0;
	node->prev = prev;
	node->next = NULL;
	node->index = index;
	return (node);
}

// searches for unclosed quotes
int open_quotes(char *str)
{
	int	i;
	int tmp;

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
				return(0);
			else if (str[i] == '\0')
				return(1);
		}
		i++;
	}
	return (0);
}

// searches for unclosed prentheses
int	open_brackets(char *str)
{
	int	i;
	int bracket;

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
			i++;
			while (str[i] != '\0' && str[i] != bracket)
				i++;
			if (str[i] == bracket && bracket == 41)
				return(0);//brackets closed and correct
			if ((str[i] == bracket && bracket == 40) || (str[i] == '\0' && bracket == 41))
				return (41);//closed the other way around or unclosed )
			else if (str[i] == '\0' && bracket == 40)
				return(40);//unclosed (
		}
		i++;
	}
	return (0);
}

// searches for unclosed curly braces
int	open_curly(char *str)
{
	int	i;
	int curly;

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
			i++;
			while (str[i] != '\0' && str[i] != curly)
				i++;
			if (str[i] == curly && curly == 125)
				return (0);//braces closed and correct
			else if ((str[i] == curly && curly == 123) || (str[i] == '\0' && curly == 125))
				return (125);//closed the other way around or unclosed }
			else if (str[i] == '\0' && curly == 123)
				return (123);//unclosed {
		}
		i++;
	}
	return (0);
}
