#include "minishell.h"

//for debugging 
void print_redir_list(t_redir *head) {
    t_redir *current = head;
    while (current != NULL) {
        printf("Type: %d, String: %s\n", current->type, current->str);
        current = current->next;
    }
}
//for debugghi
void print_simple_cmds_list(t_shell *shell) 
{
    t_simple_cmds *current = shell->cmds;
    int i = -1;
    while (current != NULL) 
	{
		i = -1;
        ft_printf("Index: %d, Amount of Commands: %d \n", current->index, shell->amount_of_cmds);
   		while (current->str[++i] != NULL)
        	ft_printf("String[%d]:%s\n", i, current->str[i]);
    	print_redir_list(current->redir); // Print redir list for each t_simple_cmds node
    	current = current->next;
    }
}

char    **arrdup(char **env)
{
    int     i;
    char    **ret;
    i = 0;
    while (env[i])
        i++;
    ret = ft_calloc(sizeof(char **), i + 2);
    if (!ret)
        return (NULL);
    i = 0;
    while (env[i] != NULL)
    {
        ret[i] = ft_strdup(env[i]);
        if (ret[i] == NULL)
            return (NULL);
        i++;
    }
    ret[i] = NULL;
    return (ret);
}

/*t_shell	*init_shell(char *str, char **envp)
{
	t_shell	*shell;
	t_lexer	*lexer;

	shell = ft_calloc(sizeof(t_shell *), 1);
	if (!shell)
		return (NULL);
	lexer = ft_lexer(str);
	if (!lexer)
		return (NULL);
	shell->env = arrdup(envp);
	if (!shell->env)
		return (NULL);
	shell->locvars = ft_calloc(sizeof(char *), 1);
	shell->cmds = ft_parser(lexer);
	if (!shell->cmds)
		return (NULL);
	return (shell);
}
*/
int	test_init_shell(t_shell *shell, char *str)
{
	char			**split_str;

	split_str = ft_split(str, ' ');
    shell->cmds = ft_calloc(sizeof(t_simple_cmds), 1);
	if (!shell->cmds)
		return (1);
	shell->amount_of_cmds = 1;
    shell->cmds->str = split_str;
    shell->cmds->builtin = &ft_export;
	shell->cmds->index = 0;
	shell->cmds->redir = ft_calloc(sizeof(t_redir), 1);
	if (!shell->cmds->redir)
		return (1);
	/*
	shell->cmds->redir->type = l_out;
	shell->cmds->redir->str = ft_strdup("out.txt");
	shell->cmds->redir->next = NULL;
	*/
	shell->cmds->next = NULL;
	return (0);
}

t_shell	*minishell_loop(t_shell *shell)
{
	char			*prompt;
	char			*str;

	prompt = PROMPT;
	prompt = variable_expansion(prompt, shell->env);
	str = readline(prompt);
	//add_history(str);
	if(test_init_shell(shell, str) != 0)
		return (NULL);
	if (!shell)
		return (NULL);
	print_simple_cmds_list(shell);
	//saves the first node as the 1st command
	//should have been validatet at this point already-->
	//so we know it's a command. Anything else should have returned error in the lexer
	//add check to exclude empty str in between quotations
	//parser
	//scans the saved tokens for an environment variable and substitutes(expand) it with its value
	expander(shell);
	print_simple_cmds_list(shell);
	printf("succesfully printed cmds list\n");
	executor(shell);
	//free_lexer(lexer);
	free(prompt);
	free(str);//frees the readline
	free_simple_commands(shell->cmds);
	return (shell);
}

int	main(int ac, char **av, char **envp)
{
	t_shell	*shell;

	shell = ft_calloc(sizeof(t_shell), 1);
	if (!shell)
		return (-1);
	shell->env = arrdup(envp);
	(void)ac;
	(void)av;
	while (1)
	{
		//if exitsignal
		//	free shell;
		minishell_loop(shell);
	}
}
