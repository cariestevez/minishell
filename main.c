/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emollebr <emollebr@student.42berlin.d      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/31 15:01:03 by emollebr          #+#    #+#             */
/*   Updated: 2023/10/31 15:03:41 by emollebr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// o asifudgldsakjfhksj

#include "parser.h"

int free_lexer(t_lexer *lexer)
{
    t_lexer *tmp;

    while (lexer)
    {
        tmp = lexer->next;
        free(lexer);
        lexer = tmp;
    }
    return (0);
}

void    print_cmds(t_simple_cmds *cmds)
{
    int i;

    i = 0;
    while (cmds->str[i])
    {
        ft_printf("cmd string is %s\n", cmds->str[i]);
        i++;
    }
    //ft_printf("infile is %s", cmds->in->str);
    //ft_printf("outfile is %s", cmds->in->str);
}

int minishell_loop()
{
    char        *str;
    t_lexer         *lexer;
    const char    *prompt;
    t_simple_cmds   *cmds;

    cmds = (t_simple_cmds *)malloc(sizeof(t_simple_cmds));
    cmds->str = malloc(sizeof(char *) * 2);
    prompt = "pRoMpT hErE ~(*__*)~> ";
    str = readline (prompt);
    lexer = ft_lexer(str);
    cmds->str[0] = lexer->str;
    expander(cmds);
    free_lexer(lexer);
    return (0);

}

int main()
{
    while (1)
        minishell_loop();
}
