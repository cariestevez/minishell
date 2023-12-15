/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utils2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emollebr <emollebr@student.42berlin.d      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/15 14:45:44 by emollebr          #+#    #+#             */
/*   Updated: 2023/12/15 14:46:11 by emollebr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include <minishell.h>

t_lexertype	get_key(char *str)
{
	int	i;

	i = 0;
	if (str[i] == '<')
	{
		if (str[i + 1] == '<')
			return (l_heredoc);
		else
			return (l_in);
	}
	if (str[i] == '>')
	{
		if (str[i + 1] == '>')
			return (l_append);
		else
			return (l_out);
	}
	if (str[i] == '|')
		return (l_pipe);
	return (l_non_op);
}

int	skip_spaces(char *str, int i)
{
	while (str[i] != '\0' && str[i] == ' ')
		i++;
	return (i);
}
