#include "parser.h"

// initializes the new nodes of the list
t_lexer *new_node(t_lexer *prev)
{
	t_lexer *node;
	
	node = (t_lexer *)malloc(sizeof(t_lexer));
	if (node == NULL)
		return (NULL);
	node->token = NULL;
	node->key = 0;
	node->prev = prev;
	node->next = NULL;
	if (!prev)
		node->index = 0;
	else
		node->index = prev->index + 1;
	return (node);
}

//for debugging
void print_lex(t_lexer *lexer)
{
	ft_printf("index: %d\n", lexer->index);
	if (lexer->token)
		ft_printf("word: %s\n", lexer->token);
	if (lexer->key)
	{
		if (lexer->key == l_pipe)
			ft_printf("key: pipe\n");
		if (lexer->key == l_space)
			ft_printf("key: space\n");
		else
			ft_printf("key: %u\n", lexer->key);
	}
	write(1, "\n", 1);
	return ;
}

void    free_array(char **str)
{
	int i;

	i = -1;
	while (str[++i])
		free(str[i]);
	return ;
}

// checks if it's space, pipe, operand, quotations... Else -is command or file name- ret 0.

t_lexertype get_key(char *str, int i)
{
	if (str[i] == '<')
	{
		if (str[i + 1] == '<')
			return (l_append);
		else
			return (l_in);
	}
	if (str[i] == '>')
		return (l_out);
	if (str[i] == '|')
		return (l_pipe);
	if (str[i] == '"')
		return (l_dquote);
	if (str[i] == '\'')
		return (l_quote);
	if (str[i] == ' ')
		return (l_space);
	return (l_non_op);
}

//searches for the closing quotation mark and returns its position
int if_quotes(char *str, int i)
{
	int tmp;

	tmp = 0;
	if (str[i] == 34 || str[i] == 39)
	{
		tmp = str[i];
		i++;
		while (str[i] != tmp && str[i] != '\0')
			i++;
		i++;
	}
	if (str[i] == '\0')
		return(tmp);
	return (i);
}

// separes the words(commands/file names etc), operands or pipes in nodes, creating a list
//BUG!: if the command line starts with one of the keys (|, >, < etc) it leaves the 1st node empty
//and saves the 'key' as part of the next one together with the command str
//instead of returning an error bc of invalid input
//BUG!: if quotation is open saves the whole command line as 1 single node instead of returning an error bc of invalid input
//ADD: if \ or ; or other special chars --> as subject: not interpret == ignore?
//ADD: don't save spaces or empty strings (""), just skip
//ADD: delete (don't save) the quotations marks around a command or file name when saving it
//if the whole command line is in between quotation marks->invalid input
int    parse_command_line(t_lexer *lexer, char *str)
{
	int i;
	int start;

	i = 0;
	while (str[i] && str[i] == ' ')
		i++;
	start = i;	
	while (str[i] != '\0')
	{
		//returns the position of the end of the quotations to save its content as a str
		i = if_quotes(str, i);
		if (get_key(str, i) != l_non_op && i != start)
		{
			lexer->token = ft_substr(str, start, i - start);
			// print for debuggin
			print_lex(lexer);
			lexer->next = new_node(lexer);
			lexer = lexer->next;
			if (!lexer)
			{
				free_lexer(lexer);
				free(str);
				return (-1);
			}
			while (get_key(str, i) == l_space)
				i++;
			start = i;
			//lexer->key = get_key(str, i);
		}

		i++;
	}
	lexer->token = ft_substr(str, start, i - start);
	print_lex(lexer);
	if (!lexer)
	{
		free_lexer(lexer);
		free(str);
		return (-1);
	}
	return (1);
}

//initializes the list and saves the pointer to the 1st node (head)
t_lexer	*ft_lexer(char *str)
{
	t_lexer     *lexer;
	t_lexer     *head;
	int           i;
	int           start;

	i = 0;
	start = 0;
	lexer = new_node(NULL);
	if (!lexer)
	{
		free(str);
		exit(1);// use perror or strerror instead?!
	}
	head = lexer;
	if (!parse_command_line(lexer, str))
		return (NULL);
	//free(str);-->being freed in the main for now
	return (head);
}
