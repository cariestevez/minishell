/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emollebr <emollebr@student.42berlin.d      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/15 14:39:22 by emollebr          #+#    #+#             */
/*   Updated: 2023/12/15 14:39:26 by emollebr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

void	reset_rl(int signum)
{
	(void)signum;
	write(1, "\n", 1);
	g_last_exit = 130;
	rl_replace_line("", 0);
	rl_on_new_line();
	rl_redisplay();
}

void	display_new_line(int signum)
{
	if (signum == SIGQUIT)
	{
		ft_printf("Quit (core dumped)");
		g_last_exit = 131;
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
