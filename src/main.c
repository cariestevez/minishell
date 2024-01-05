/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cestevez <cestevez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/13 22:20:02 by emollebr          #+#    #+#             */
/*   Updated: 2024/01/04 22:39:55 by cestevez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	g_last_exit;

void	handle_signals(int sig)
{
	if (sig == SIGINT)
	{
		write(STDERR_FILENO, "\n", 1);
		rl_on_new_line();
		rl_replace_line("", 0);
		if (g_last_exit != 155)
		{
			rl_redisplay();
		}
		g_last_exit = 130;
	}
}

void	handle_signals_cmd(int sig)
{
	if (sig == SIGINT)
		write(STDERR_FILENO, "\n", 1);
	else if (sig == SIGQUIT)
		write(STDERR_FILENO, "Quit(core dumped)\n", 18);
}

int	init_signals_cmd(void)
{
	struct sigaction	sa_signals;

	sa_signals.sa_handler = handle_signals_cmd;
	sigemptyset(&sa_signals.sa_mask);
	sa_signals.sa_flags = 0;
	signal(SIGINT, handle_signals_cmd);
	signal(SIGQUIT, handle_signals_cmd);
	signal(SIGQUIT, SIG_DFL);
	if (sigaction(SIGINT, &sa_signals, NULL) == -1)
	{
		perror("sigaction");
		return (1);
	}
	if (sigaction(SIGQUIT, &sa_signals, NULL) == -1)
	{
		perror("sigaction");
		return (1);
	}
	return (0);
}

int	init_signals(void)
{
	struct sigaction	sa_signals;

	sa_signals.sa_handler = handle_signals;
	sigemptyset(&sa_signals.sa_mask);
	sa_signals.sa_flags = 0;
	signal(SIGQUIT, SIG_IGN);
	if (sigaction(SIGINT, &sa_signals, NULL) == -1)
	{
		perror("sigaction");
		return (1);
	}
	return (0);
}

char	**arrdup(char **env)
{
	int		i;
	char	**ret;

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

int	empty_str(char *str)
{
	int	i;

	i = 0;
	if (!str)
	{
		ft_printf("exit\n");
		return (g_last_exit += 1000);
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

char	*get_input(t_shell *shell)
{
	char		*prompt;
	char		*str;

	str = NULL;
	prompt = check_for_variables(ft_strdup(PROMPT), shell);
	str = readline(prompt);
	signals_non_interactive();
	if (empty_str(str))
	{
		free(prompt);
		return (NULL);
	}
	add_history(str);
	free(prompt);
	return (str);
}

int	minishell_loop(t_shell *shell)
{
	t_lexer			*lexer;
	t_simple_cmds	*head;
	char			*str;

	lexer = NULL;
	head = NULL;
	str = get_input(shell);
	if (str == NULL)
		return (g_last_exit);
	lexer = ft_lexer(str);
	free(str);
	str = NULL;
	if (lexer == NULL)
		return (errno);
	shell->cmds = ft_parser(lexer, shell);
	head = shell->cmds;
	free_lexer(lexer);
	if (shell->cmds != NULL)
		g_last_exit = executor(shell);
	free_cmds(head);
	return (g_last_exit);
}

int	main(int ac, char **av, char **envp)
{
	t_shell	*shell;

	g_last_exit = 0;
	shell = ft_calloc(sizeof(t_shell), 1);
	if (!shell)
		return (-1);
	shell->env = arrdup(envp);
	(void)ac;
	(void)av;
	while (1)
	{
		signals_interactive();
		// init_signals();
		g_last_exit = minishell_loop(shell);
		if (g_last_exit >= 1000)
		{
			g_last_exit -= 1000;
			break ;
		}
	}
	free_char_arr(shell->env);
	free(shell);
	rl_clear_history();
	return (g_last_exit);
}
