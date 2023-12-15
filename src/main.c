/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emollebr <emollebr@student.42berlin.d      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/13 22:20:02 by emollebr          #+#    #+#             */
/*   Updated: 2023/12/13 22:20:03 by emollebr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "minishell.h"

int g_last_exit;

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

sig_atomic_t	minishell_loop(t_shell *shell)
{
	t_lexer			*lexer;
	t_simple_cmds	*head;
	char			*str;

	lexer = NULL;
	head = NULL;
	str = get_input(shell);
	if (str == NULL)
		return (g_last_exit);
	lexer = ft_lexer(str, g_last_exit);
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
		g_last_exit = minishell_loop(shell);
		if (g_last_exit >= 1000)
			break ;
	}
	free_char_arr(shell->env);
	free(shell);
	rl_clear_history();
	return (errno);
}
