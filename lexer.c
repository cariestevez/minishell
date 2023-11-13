#include "minishell.h"

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
int if_quotes(char *str, int first_position)
{
	char 	looking_for;
	int		i;

	i = first_position + 1;
	if (str[first_position] == 34 || str[first_position] == 39)
	{
		looking_for = str[first_position];
		while (str[i] != looking_for && str[i] != '\0')
			i++;
		ft_printf("found closing quotes at %c, index %d\n", str[i], i);
	}
	if (str[i] == '\0')
		return (first_position);
	return (i + 1);
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
int	get_token(char *str, int i, t_lexer *lexer)
{
	int	start;

	start = i;
	//check for operators and return if true
	if (get_key(str, i) == l_pipe || get_key(str, i) == l_in 
			|| get_key(str, i) == l_out || get_key(str, i) == l_append)
	{
		lexer->key = get_key(str, i);
		return (i);
	}
	else if (get_key(str, i) == l_append)
	{
		lexer->key = get_key(str, i);
		return (i + 1);
	}
	//returns the position of the end of the quotations to save its content as a str
	i = if_quotes(str, i);
	//finds end of token and duplicates
	while (get_key(str, i) == l_non_op && str[i] != '\0')
			i++;
	lexer->token = ft_substr(str, start, i - start);
	lexer->key = l_word;
	return (i);
}

t_lexer    *parse_command_line(t_lexer *lexer, char *str)
{
	int i;
	int start;
	t_lexer	*head;

	i = 0;
	head = lexer;
	while (str[i] && str[i] == ' ')
		i++;
	start = i;	
	while (str[i] != '\0')
	{
		//skip leading whitespace
		while ((get_key(str, i) - 1) == l_space)
				i++;
		if (!str[i])
			break ;
		i = get_token(str, i, lexer);
		if (!str[i])
			break ;
		// print for debuggin
		print_lex(lexer);
		lexer->next = new_node(lexer);
		lexer = lexer->next;
		if (!lexer)
		{
			//free_lexer(lexer);
			free(str);
			return (NULL);
		}
		i++;
		ft_printf("end of loop, position is i=%d: %c\n", i, str[i]);
	}
	lexer->token = ft_substr(str, start, i - start);
	print_lex(lexer);
	if (!lexer)
	{
		//free_lexer(lexer);
		free(str);
		return (NULL);
	}
	return (head);
}

//initializes the list and saves the pointer to the 1st node (head)
t_lexer	*ft_lexer(char *str)
{
	t_lexer     *lexer;
	t_lexer     *head;

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
