#include "minishell.h"

void	free_char_arr(char **tab)
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

void    free_int_arr(int  **arr)
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

	while (lexer != NULL)
	{
		tmp = lexer->next;
		free(lexer->token);
		lexer->token = NULL;
		free(lexer);
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
        free_char_arr(cmds->str);
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