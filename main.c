#include "minishell.h"

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

char    **arrdup(char **env)
{
    int     i;
    char    **ret;
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

int	minishell_loop(char **envp)
{
	char			*str;
	t_lexer			*lexer;
	t_simple_cmds	*cmds;
	char			**env_cpy;

	env_cpy = arrdup(envp);
	cmds = (t_simple_cmds *)malloc(sizeof(t_simple_cmds));
	cmds->str = malloc(sizeof(char *) * 2);
	//printf statement changes the color of the stdout
	ft_printf("\033[35m");
	str = readline((expand_var(PROMPT, 12, env_cpy)));
	//this changes it back to white/black
	ft_printf("\033[0m");

	//add_history(str);
	lexer = ft_lexer(str);
	cmds = ft_parser(lexer);
	//saves the first node as the 1st command
	//should have been validatet at this point already-->
	//so we know it's a command. Anything else should have returned error in the lexer
	//add check to exclude empty str in between quotations
	//parser
	//scans the saved tokens for an environment variable and substitutes(expand) it with its value
	//expander(cmds);
	executor(cmds);
	//free_lexer(lexer);
	free(str);//frees the readline
	free(cmds->str);
	free(cmds);
	return (0);
}

int	main(int ac, char **av, char **envp)
{
	(void)ac;
	(void)av;
	while (1)
		minishell_loop(envp);
}