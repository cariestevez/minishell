/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_echo.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emollebr <emollebr@student.42berlin.d      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/13 20:22:46 by emollebr          #+#    #+#             */
/*   Updated: 2023/12/13 20:22:49 by emollebr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	print_args(char **display, int i)
{
	while (display[i] != NULL)
	{
		ft_putstr_fd(display[i], 1);
		i++;
		if (display[i] != NULL)
			ft_putchar_fd(' ', 1);
	}
	return (0);
}

int	ft_echo(t_shell *shell, t_simple_cmds *cmd)
{
	int		flag_check;

	(void)shell;
	if (!cmd->str[1])
	{
		write(1, "\n", 1);
		return (0);
	}
	if (ft_strncmp(cmd->str[1], "-n", BUFFER) == 0)
	{
		if (!cmd->str[2])
			return (0);
		flag_check = 2;
	}
	else
		flag_check = 1;
	print_args(cmd->str, flag_check);
	if (flag_check == 1)
		write(1, "\n", 1);
	return (0);
}
