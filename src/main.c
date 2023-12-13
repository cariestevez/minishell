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

int	empty_str(char *str, t_shell *shell)
{
	int	i;

	i = 0;
	if (!str)
	{
		ft_printf("exit\n");
		return (shell->exitcode += 1000);
	}
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

// void	sig_handler_parent(int signum)
// {
// 	signal(SIGINT, SIG_IGN);
// 	if (signum == SIGINT)
// 	{
// 		write(1, "\n", 1);
// 		g_last_exit = 130;
// 	}
// }

// void	sig_handler_child(int signum)
// {
// 	int	pid;

// 	pid = getpid();
// 	if (signum == SIGINT)
// 	{
// 		g_last_exit = 130;
// 		kill()
// 	}
// }

// void	sig_handler_heredoc(int signum)
// {
// 	signal(SIGINT, SIG_IGN);
// 	if (signum == SIGINT)
// 	{
// 		g_last_exit = 130;
// 	}
// }
void reset_rl(int signum)
{
	(void)signum;

	write(1, "\n", 1);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}

void	display_new_line(int signum)
{
	if (signum == SIGQUIT)
		ft_printf("Quit (core dumped)");
	write(1, "\n", STDERR_FILENO);
	rl_on_new_line();
}

void	signals_interactive(void)
{
	signal(SIGINT, reset_rl);
	signal(SIGQUIT, SIG_IGN);
}

void	signals_non_interactive(void)
{
	signal(SIGINT, display_new_line);
	signal(SIGQUIT, display_new_line);
}

void minishell_loop(t_shell *shell)
{
	char			*str;
	char			*prompt;
	t_lexer			*lexer;
	t_simple_cmds	*head;

	str = NULL;
	lexer = NULL;
	head = NULL;
	prompt = check_for_variables(ft_strdup(PROMPT), shell);
	str = readline(prompt);
	signals_non_interactive();
	if (empty_str(str, shell))
		return (free(prompt));
	add_history(str);
	lexer = ft_lexer(str);
//	print_lex(lexer);
	free(str);
	str = NULL;
	if (lexer == NULL)
	{
		shell->exitcode = errno;
		return; //(shell);
	}
	shell->cmds = ft_parser(lexer, shell);
	head = shell->cmds;
	if (shell->cmds != NULL)
	{
		shell->exitcode = expander(shell);
		shell->exitcode = executor(shell);
	}
	free_on_succes(head, lexer, prompt);
	return;// (shell);
}

int	main(int ac, char **av, char **envp)
{
	t_shell	*shell;

	shell = ft_calloc(sizeof(t_shell), 1);
	if (!shell)
		return (-1);
	g_last_exit = 0;
	shell->env = arrdup(envp);
	shell->exitcode = 0;
	(void)ac;
	(void)av;
	while (1)
	{
		signals_interactive();
		minishell_loop(shell);
		if (shell->exitcode >= 1000)
			break ;
	}
	free_char_arr(shell->env);
	free(shell);
	rl_clear_history();
	return (errno);
}
