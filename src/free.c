#include "minishell.h"

void	free_tab(char **tab)
{
	int	i;

	i = 0;
	while (tab[i])
	{
		free(tab[i]);
		tab[i] = NULL;
		i++;
	}
	free(tab);
	tab = NULL;
}

void    free_i_array(int  **arr)
{
    int i;

    i = 0;
    while (arr[i] != NULL)
    {
        free(arr[i]);
		arr[i] = NULL;
        i++;
    }
    free(arr);
	arr = NULL;
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

void    free_simple_commands(t_simple_cmds *cmds)
{
	t_redir	*tmp;

	tmp = NULL;
    while (cmds)
    {
        free_tab(cmds->str);
		cmds->str = NULL;
		while (cmds->redir != NULL)
		{
			free(cmds->redir->str);
        	tmp = cmds->redir;
			cmds->redir = cmds->redir->next;
			free(tmp);
		}
		if (cmds->index > 0)
            free(cmds->prev);
        if (cmds->next == NULL)
        {
            free(cmds);
            return ;
        }
        cmds = cmds->next;
    }
}