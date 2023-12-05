#include "parser.h"

//-----------------------------------------------------------------------
//for debugging. DELETE FILE

void print_lex(t_lexer *lexer)
{
	while (lexer)
	{
		ft_printf("index: %d\n", lexer->index);
		if (lexer->token)
			ft_printf("token: %s\n", lexer->token);
		if (lexer->key)
		{
			if (lexer->key == l_pipe)
				ft_printf("key: PIPE\n");
			else if (lexer->key == l_in)
				ft_printf("key: REDIRECTION input\n");
			else if (lexer->key == l_out)
				ft_printf("key: REDIRECTION output\n");
			else if (lexer->key == l_heredoc)
				ft_printf("key: heredoc\n");
			else if (lexer->key == l_append)
				ft_printf("key: REDIR append\n");
			else if (lexer->key == l_non_op)
				ft_printf("key: non operator\n");
			else
				ft_printf("key: %u\n", lexer->key);
		}
		write(1, "\n", 1);
		lexer = lexer->next;
	}
	return ;
}

void print_redir_list(t_redir *head)
{
    t_redir *current = head;
    
	while (current != NULL)
	{
        printf("Type: %d, String: %s\n", current->type, current->str);
        current = current->next;
    }
}

void print_simple_cmds_list(t_shell *shell) 
{
    t_simple_cmds *current = shell->cmds;
    int i = 0;
	ft_printf("hello from print_simple_cmds\n");
	if (!shell->cmds)
		ft_printf("no commands in shell struct\n");
    while (current != NULL) 
	{
		i = 0;
        ft_printf("Index: %d, Amount of Commands: %d \n", current->index, shell->amount_of_cmds);
   		while (current->str && current->str[i] != NULL)
		{
        	ft_printf("String[%d]:%s\n", i, current->str[i]);
			i++;
		}
		if (current->redir)
    	 	print_redir_list(current->redir);
    	current = current->next;
    }
}

// void	print_cmds(t_simple_cmds *cmds)
// {
// 	int	i;

// 	i = 0;
// 	while (cmds->str[i])
// 	{
// 		ft_printf("cmd string is %s\n", cmds->str[i]);
// 		i++;
// 	}
// 	//ft_printf("infile is %s", cmds->in->str);
// 	//ft_printf("outfile is %s", cmds->in->str);
// }
//-----------------------------------------------------------------------
