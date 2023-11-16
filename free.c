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

	//t_lexer *head;
	while (lexer)
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

void    free_and_exit(t_shell *shell, int **fd, int exitcode)
{
    //this function call will close ALL fds
	if (fd != NULL)
	{
		close_unneccesary_fds(fd, shell->amount_of_cmds + 1, shell->amount_of_cmds);
	 	free_array(fd);
	}
	free_simple_commands(shell->cmds);
    free_tab(shell->env);
    free_tab(shell->locvars);
	free(shell);
	//perror(get_exit_msg(exitcode));
	exit(exitcode);
}