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

//add redir* free when adding to struct!
void	free_cmds(t_simple_cmds *cmd_node)
{
	t_simple_cmds	*temp;

	temp = NULL;
	while (cmd_node != NULL)
	{
		temp = cmd_node->next;
		cmd_node->next = NULL;
		cmd_node->prev = NULL;
		// if (cmd_node->builtin != NULL)
		// {
		// 	free(cmd_node->builtin);
		// 	cmd_node->builtin = NULL;
		// }
		if (cmd_node->redir != NULL)
		{
			free(cmd_node->redir);
			cmd_node->redir = NULL;
		}
		free_cmd_array(cmd_node->str);
		cmd_node->str = NULL;
		free(cmd_node);
		cmd_node = temp;	
	}
}

//searches for pipes to find out how many simple cmds there are
int	count_cmds(t_lexer	*lexer)
{
	int	cmds;

	cmds = 1;
	while (lexer != NULL)
	{
		if (lexer->key == l_pipe)
		{
			if (lexer->prev == NULL)
			{
				ft_printf("syntax error near unexpected token `|'");
				return (0);
			}
			if (lexer->next == NULL)//count a phantom cmd?->allocating for an empty shell->cmd->str. problem? if array should be NULL terminated there might be 2 NULL terminated ptrs
				ft_printf("last token is `|'. Implement?!\n");
			cmds++;
		}
		lexer = lexer->next;
	}
	ft_printf("count cmds = %i\n", cmds);//delete!
	return (cmds);
}

// initializes the new nodes of the list
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
	if (!node->str)
		return (NULL);
	node->next = NULL;
	return(node);
}

// t_shell *init_shell()
// {
// 	t_shell	*shell;

// 	shell = (t_shell *)malloc(sizeof(t_shell));
// 	if (!shell)
// 		return (NULL);
// 	shell->cmds = NULL;
// 	shell->amount_of_cmds = 0;
// 	shell->env = NULL;
// 	return (shell);
// }
