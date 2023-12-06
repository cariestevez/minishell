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

void	free_on_succes(t_shell *shell, t_lexer *lexer, char *prompt)
{
	free_lexer(lexer);
	free(prompt);
	free_simple_commands(shell->cmds);
	shell->cmds = NULL;
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

	prompt = variable_expansion(prompt, shell);
	str = readline(prompt);
	if (empty_str(str))
		return (shell);
	add_history(str);
	lexer = ft_lexer(str);
	free(str);
	str = NULL;
	if (lexer == NULL)
		return (shell);
	shell->cmds = ft_parser(lexer, shell);
	if (shell->cmds == NULL)
		return (free_lexer(lexer), shell);
	shell->exitcode = expander(shell);
	shell->exitcode = executor(shell);
	free_on_succes(shell, lexer, prompt);
	return (shell);
}

int	main(int ac, char **av, char **envp)
{
	t_shell	*shell;

	shell = ft_calloc(sizeof(t_shell), 1);
	if (!shell)
		return (-1);
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
	rl_clear_history();
	return (0);
}
