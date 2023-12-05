#include "minishell.h"

void	free_tab(char **tab)
{
	int	i;

	i = 0;
	while (tab[i])
	{
		free(tab[i]);
		i++;
	}
}

void    free_array(int  **arr)
{
    int i;

    i = 0;
    while (arr[i] != NULL)
    {
        free(arr[i]);
        i++;
    }
    free(arr);
}

int	free_lexer(t_lexer *lexer)
{
	t_lexer	*tmp;

	while (lexer != NULL)
	{
		tmp = lexer->next;
		free(lexer);
		lexer = tmp;
	}
	free(lexer);
	return (0);
}

void    free_simple_commands(t_simple_cmds *cmds)
{
	t_redir	*tmp;

	tmp = NULL;
    while (cmds)
    {
        free_tab(cmds->str);
		while (cmds->redir)
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