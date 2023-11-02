#include "parser.h"

int	free_lexer(t_lexer *lexer)
{
	t_lexer	*tmp;

	//t_lexer *head;
	while (lexer)
	{
		tmp = lexer->next;
		free(lexer);
		lexer = tmp;
	}
	free(lexer);
	return (0);
}

void	print_cmds(t_simple_cmds *cmds)
{
	int	i;

	i = 0;
	while (cmds->str[i])
	{
		ft_printf("cmd string is %s\n", cmds->str[i]);
		i++;
	}
	//ft_printf("infile is %s", cmds->in->str);
	//ft_printf("outfile is %s", cmds->in->str);
}

int	minishell_loop(void)
{
	char			*str;
	t_lexer			*lexer;
	const char		*prompt;
	t_simple_cmds	*cmds;

	cmds = (t_simple_cmds *)malloc(sizeof(t_simple_cmds));
	cmds->str = malloc(sizeof(char *) * 2);
	prompt = "pRoMpT hErE ~(*__*)~> ";
	str = readline(prompt);
	//add_history(str);
	lexer = ft_lexer(str);
	//saves the first node as the 1st command
	//should have been validatet at this point already-->
	//so we know it's a command. Anything else should have returned error in the lexer
	//add check to exclude empty str in between quotations
	//parser
	//scans the saved tokens for an environment variable and substitutes(expand) it with its value
	//expander(cmds);
	free_lexer(lexer);
	free(str);//frees the readline
	free(cmds->str);
	free(cmds);
	return (0);
}

int	main(void)
{
	while (1)
		minishell_loop();
}