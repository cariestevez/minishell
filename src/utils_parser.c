#include "parser.h"

void	free_cmd_array(char **str)
{
	int	i;

	i = 0;
	if (str != NULL)
	{
		while (str[i] != NULL)
		{
			free(str[i]);
			str[i] = NULL;
			i++;
		}
		free(str);
	}
}

void	free_cmds(t_simple_cmds *cmd_node)
{
	t_simple_cmds	*temp;
	t_redir			*redir_tmp;

	temp = NULL;
	redir_tmp = NULL;
	while (cmd_node != NULL)
	{
		temp = cmd_node->next;
		cmd_node->next = NULL;
		cmd_node->prev = NULL;
		if (cmd_node->builtin != NULL)
		{
			free(cmd_node->builtin);
			cmd_node->builtin = NULL;
		}
		while (cmd_node->redir != NULL)
		{
			free(cmd_node->redir->str);
			cmd_node->redir->str = NULL;
			redir_tmp = cmd_node->redir->next;
			free(cmd_node->redir);
			cmd_node->redir = redir_tmp;
		}
		free_cmd_array(cmd_node->str);
		cmd_node->str = NULL;
		free(cmd_node);
		cmd_node = temp;
	}
}

//searches for pipes to find out how many simple cmds there are
//returns syntax error if pipe is on 1st or last position
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
				ft_printf("syntax error near unexpected token `|'");
				return (0);
			}
			cmds++;
		}
		lexer = lexer->next;
	}
	return (cmds);
}

// initializes the new nodes of the command list
t_simple_cmds	*new_cmd_node(t_simple_cmds *prev)
{
	t_simple_cmds *node;
	
	node = (t_simple_cmds *)malloc(sizeof(t_simple_cmds));
	if (node == NULL)
		return (NULL);
	node->str = NULL;
	node->builtin = NULL;
	node->redir = NULL;
	node->prev = prev;
	node->next = NULL;
	if (!prev)
		node->index = 0;
	else
		node->index = prev->index + 1;
	return (node);
}

t_redir *new_redir_node (char *file, t_lexertype type)
{
	t_redir	*node;

	node = (t_redir *)ft_calloc(sizeof(t_redir), 1);
	if (!node)
		return (NULL);
	node->type = type;
	node->str = ft_strdup(file);
	ft_printf("parsing redirection %s\n", file);
	if (!node->str)
		return (NULL);
	node->next = NULL;
	return(node);
}
