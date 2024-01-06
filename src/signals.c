/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cestevez <cestevez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/15 14:39:22 by emollebr          #+#    #+#             */
/*   Updated: 2024/01/05 13:22:57 by cestevez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

void	display_new_line(int signum)
{
	if (signum == SIGQUIT)
	{
		g_last_exit = 131;
		ft_printf("Quit (bash would dump the core)");
	}
	if (signum == SIGINT)
		g_last_exit = 130;
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

void	signals_heredoc(void)
{
	signal(SIGQUIT, SIG_IGN);
	signal(SIGINT, redisplay_heredoc);
}

void	redisplay_heredoc(int signum)
{
	(void)signum;
	g_last_exit = 130;
	ioctl(STDIN_FILENO, TIOCSTI, "\n");
}
