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
	free_lexer(lexer);
	free(prompt);
	free_cmds(cmds);
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

t_shell	*minishell_loop(t_shell *shell)
{
	char			*str;
	t_lexer			*lexer;
	char			*prompt;
	t_simple_cmds	*head;

	str = NULL;
	lexer = NULL;
	head = NULL;
	prompt = check_for_variables(ft_strdup(PROMPT), shell);
	str = readline(prompt);
	if (empty_str(str))
		return (shell);
	add_history(str);
	lexer = ft_lexer(str);
	print_lex(lexer);
	free(str);
	str = NULL;
	if (lexer == NULL)
	{
		shell->exitcode = errno;
		return (shell);
	}
	shell->cmds = ft_parser(lexer, shell);
	head = shell->cmds;
	if (shell->cmds != NULL)
	{
		shell->exitcode = expander(shell);
		shell->exitcode = executor(shell);
	}
	free_on_succes(head, lexer, prompt);
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
		shell = minishell_loop(shell);
		if (shell->exitcode >= 1000)
			break ;
	}
	free_char_arr(shell->env);
	free(shell);
	rl_clear_history();
	return (errno);
}
