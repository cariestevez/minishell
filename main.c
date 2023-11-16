#include "minishell.h"

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
t_shell	*test_init_shell(char *str, char **envp)
{
	t_shell         *shell;
	char			**split_str;
    int                 i;

	split_str = ft_split(str, ' ');
    shell = malloc(sizeof(t_shell *));
    shell->cmds = malloc(sizeof(t_simple_cmds *));
    shell->env = arrdup(envp);
    ft_printf("env before cd:\n");
    i = 0;
    while (shell->env[i])
    {
        ft_printf("%s\n", shell->env[i]);
        i++;
    }
    shell->cmds->str = split_str;
    shell->cmds->builtin = &ft_cd;
    i = 0;
    while (shell->env[i])
    {
        ft_printf("%s\n", shell->env[i]);
        free(shell->env[i]);
        i++;
    }
	return (shell);
}

int	minishell_loop(char **envp)
{
	char			*prompt;
	char			*str;
	t_shell			*shell;

	prompt = PROMPT;
	prompt = variable_expansion(prompt, envp);
	str = readline(prompt);
	//add_history(str);
	shell = test_init_shell(str, envp);
	if (!shell)
		return (-1);
	
	//saves the first node as the 1st command
	//should have been validatet at this point already-->
	//so we know it's a command. Anything else should have returned error in the lexer
	//add check to exclude empty str in between quotations
	//parser
	//scans the saved tokens for an environment variable and substitutes(expand) it with its value
	//expander(cmds);
	executor(shell);
	//free_lexer(lexer);
	free(prompt);
	free(str);//frees the readline
	free_and_exit(shell, NULL, 0);
	return (0);
}

int	main(int ac, char **av, char **envp)
{
	(void)ac;
	(void)av;
	while (1)
		minishell_loop(envp);
}
