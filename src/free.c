#include "minishell.h"

void	free_on_succes(t_simple_cmds *cmds, t_lexer *lexer, char *prompt)
{
	free_lexer(lexer);
	free(prompt);
	free_cmds(cmds);
	cmds = NULL;
}

int	free_lexer(t_lexer *lexer)
{
	t_lexer	*tmp;

	ft_printf("---> free_lexer\n");
	while (lexer != NULL)
	{
		tmp = lexer->next;
		free(lexer->token);
		lexer->token = NULL;
		ft_printf("freed lexer->token\n");
		free(lexer);
		ft_printf("freed lexer node\n");
		lexer = tmp;
	}
	return (0);
}

void    free_int_arr(int  **arr)
{
    int i;

    i = 0;
	ft_printf("---> free_i_array\n");
    while (arr[i] != NULL)
    {
        free(arr[i]);
		ft_printf("freed arr[%d]\n", i);
		arr[i] = NULL;
        i++;
    }
    free(arr);
	ft_printf("freed arr\n");
	arr = NULL;
}

void	free_char_arr(char **str)
{
	int	i;

	i = 0;
	ft_printf("---> free_char_array\n");
	if (str != NULL)
	{
		while (str[i] != NULL)
		{
			free(str[i]);
			str[i] = NULL;
			ft_printf("freed cmd->str[%d], or any other char**\n", i);
			i++;
		}
		free(str);
		ft_printf("freed cmd->str, or any other char*\n");
		str = NULL;
	}
}

void	free_cmds(t_simple_cmds *cmd_node)
{
	t_simple_cmds	*temp;
	t_redir			*redir_tmp;

	temp = NULL;
	redir_tmp = NULL;
	ft_printf("---> free_cmds\n");
	while (cmd_node != NULL)
	{
		temp = cmd_node->next;
		cmd_node->next = NULL;
		cmd_node->prev = NULL;
		while (cmd_node->redir != NULL)
		{
			free(cmd_node->redir->str);
			cmd_node->redir->str = NULL;
			ft_printf("freed cmd->redir->str\n");
			redir_tmp = cmd_node->redir->next;
			free(cmd_node->redir);
			ft_printf("freed cmd->redir\n");
			cmd_node->redir = redir_tmp;
		}
		free_char_arr(cmd_node->str);
		cmd_node->str = NULL;
		ft_printf("freed cmd_array\n");
		free(cmd_node);
		cmd_node = temp;
		ft_printf("freed cmd\n");
	}
}

// void    free_simple_commands(t_simple_cmds *cmds)
// {
// 	t_redir	*tmp;

// 	tmp = NULL;
// 	ft_printf("---> free_simple_commands\n");
//     while (cmds)
//     {
//         free_tab(cmds->str);
// 		cmds->str = NULL;
// 		while (cmds->redir != NULL)
// 		{
// 			free(cmds->redir->str);
// 			ft_printf("freed cmds->redir->str\n");
//         	tmp = cmds->redir;
// 			cmds->redir = cmds->redir->next;
// 			free(tmp);
// 			ft_printf("freed cmds->redir\n");
// 		}
// 		if (cmds->index > 0)
//             free(cmds->prev);
// 		ft_printf("freed previous cmd node\n");
//         if (cmds->next == NULL)
//         {
//             free(cmds);
// 			ft_printf("freed shell->cmds\n");
//             return ;
//         }
//         cmds = cmds->next;
//     }
// }