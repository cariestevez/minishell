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

t_shell	*init_shell(char *str, char **envp)
{
	t_shell	*shell;
	t_lexer	*lexer;

	shell = ft_calloc(sizeof(t_shell *)), 1;
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

int	minishell_loop(char **envp)
{
	char			*str;
	t_lexer			*lexer;
	t_simple_cmds	*cmds;
	char			**env_cpy;

	str = readline((expand_var(PROMPT, env_cpy)));
	//add_history(str);
	shell = init_shell(str, envp);
	if (!shell)
		return (-1);
	
	//saves the first node as the 1st command
	//should have been validatet at this point already-->
	//so we know it's a command. Anything else should have returned error in the lexer
	//add check to exclude empty str in between quotations
	//parser
	//scans the saved tokens for an environment variable and substitutes(expand) it with its value
	//expander(cmds);
	executor(cmds);
	//free_lexer(lexer);
	free(str);//frees the readline
	free(cmds->str);
	free(cmds);
	return (0);
}
/*
int	main(int ac, char **av, char **envp)
{
	ft_printf(\036[0m)
	execve(getpath(cat), "welcome.txt"
	ft_printf(\033[0m);
	(void)ac;
	(void)av;
	while (1)
		minishell_loop(envp);
}*/
