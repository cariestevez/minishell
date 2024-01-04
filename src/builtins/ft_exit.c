/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emollebr <emollebr@student.42berlin.d      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/15 10:37:40 by emollebr          #+#    #+#             */
/*   Updated: 2023/12/15 10:37:41 by emollebr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_exit(t_shell *shell, t_simple_cmds *cmd)
{
	(void)cmd;
	free_cmds(shell->cmds);
	ft_putstr_fd("exit\n", 1);
	free_char_arr(shell->env);
	free(shell);
	rl_clear_history();
	exit(errno);
}
