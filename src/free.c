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
		while (cmds->redir)
		{
        	free(cmds->redir);
			cmds->redir = cmds->redir->next;
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

void	get_error_msg(int exitcode)
{
	if (exitcode == DECLARE_VAR_ERROR)
		perror("Error declaring variable");
    else if (exitcode == EXPANDER_VAR_ERROR)
		perror("Error on expander");
   	else if (exitcode ==  EXECUTOR_PIPE_ERROR)
		perror("Error on pipe");
    else if (exitcode == EXECUTOR_FORK_ERROR)
		perror("Error on fork");
    else if (exitcode == EXECUTOR_EXEC_ERROR)
		perror("Error on execution");
  	else if (exitcode ==  EXECUTOR_REDIRECTION_ERROR)
		perror("Error on redirections");
    else if (exitcode == EXECUTOR_MALLOC_ERROR)
		perror("Error on malloc");
  	 else if (exitcode == EXECUTOR_ACCESS_ERROR)
		perror("Error on accessing file");
   	else if (exitcode == EXECUTOR_HEREDOC_ERROR)
		perror("Error on heredoc");
   	else if (exitcode == STDSTREAM_RESTORE_ERROR)
		perror("Error restoring standard streams");
	return ;
}