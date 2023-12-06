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

void	free_on_succes(t_simple_cmds *cmds, t_lexer *lexer, char *prompt)
{
	ft_printf("--->free_on_success\n");
	free_lexer(lexer);
	ft_printf("back, lexer freed\n");
	free(prompt);
	ft_printf("back, prompt freed\n");
	ft_printf("AND HERE wthell is in cmds?: %s\n", cmds->str[0]);
	free_cmds(cmds);
	ft_printf("back, cmds freed\n");
	cmds = NULL;
}

int	empty_str(char *str)
{
	int	i;

	i = 0;
	if (!str)
		return (1);
	while (str[i] != '\0')
	{
		if (str[i] != 32 && !(str[i] >= 8 && str[i] <= 13))
			return (0);
		i++;
	}
	free(str);
	str = NULL;
	return (1);
}

t_shell	*minishell_loop(t_shell *shell, char *prompt)
{
	char			*str;
	t_lexer			*lexer;
	t_simple_cmds	*cmds;

	ft_printf("---> minishell_loop\n");
	str = NULL;
	lexer = NULL;
	cmds = NULL;
	prompt = variable_expansion(prompt, shell);
	str = readline(prompt);
	ft_printf("allocated prompt str\n");
	if (empty_str(str))
		return (shell);
	add_history(str);
	lexer = ft_lexer(str);
	free(str);
	ft_printf("freed prompt str\n");
	str = NULL;
	if (lexer == NULL)
		return (shell);
	cmds = ft_parser(lexer, shell);
	ft_printf("wthell is in cmds?: %s\n", cmds->str[0]);
	if (cmds == NULL)
		return (free_lexer(lexer), shell);
	shell->exitcode = expander(shell);
	shell->exitcode = executor(shell);
	free_on_succes(cmds, lexer, prompt);
	return (shell);
}

int	main(int ac, char **av, char **envp)
{
	t_shell	*shell;

	ft_printf("---> main\n");
	shell = ft_calloc(sizeof(t_shell), 1);
	if (!shell)
		return (-1);
	ft_printf("allocated shell\n");
	shell->env = arrdup(envp);
	shell->exitcode = 0;
	(void)ac;
	(void)av;
	while (1)
	{
		shell = minishell_loop(shell, PROMPT);
		ft_printf("returned to main, exitcode %d\n", shell->exitcode);
		//if exitsignal
		//{
		//	free_tab(shell->env);
		//	free(shell);
		//}
	}
	free_tab(shell->env);
	free(shell);
	ft_printf("freed shell\n");
	rl_clear_history();
	return (0);
}
