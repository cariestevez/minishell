/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_env.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emollebr <emollebr@student.42berlin.d      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/13 20:21:28 by emollebr          #+#    #+#             */
/*   Updated: 2023/12/13 20:21:32 by emollebr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_env(t_shell *shell, t_simple_cmds *cmd)
{
	int	i;

	(void)cmd;
	i = 0;
	while (shell->env[i] != NULL)
	{
		ft_putstr_fd(shell->env[i], 1);
		ft_putchar_fd('\n', 1);
		i++;
	}
	return (0);
}
