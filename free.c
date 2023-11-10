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
    while (arr[i])
    {
        free(arr[i]);
        i++;
    }
    free(arr);
}

void    free_simple_commands(t_simple_cmds *cmds)
{
    int i;

    i = 0;
    while (cmds)
    {
        free_tab(cmds->str);
        free(cmds->redir);
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