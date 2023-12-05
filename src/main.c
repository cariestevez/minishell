#include "minishell.h"

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

/*t_shell	*init_shell(char *str, char **envp)
{
	t_shell	*shell;
	t_lexer	*lexer;

	shell = ft_calloc(sizeof(t_shell *), 1);
	if (!shell)
		return (NULL);
	lexer = ft_lexer(str);
	if (!lexer)
		return (NULL);
	shell->env = arrdup(envp);
	if (!shell->env)
		return (NULL);
	shell->locvars = ft_calloc(sizeof(char *), 1);
	shell->cmds = ft_parser(lexer);
	if (!shell->cmds)
		return (NULL);
	return (shell);
}
*/
int	test_init_shell(t_shell *shell, char *str)
{
	char			**split_str;

	split_str = ft_split(str, ' ');
    shell->cmds = ft_calloc(sizeof(t_simple_cmds), 1);
	if (!shell->cmds)
		return (1);
	shell->amount_of_cmds = 1;
    shell->cmds->str = split_str;
	if (ft_strncmp(shell->cmds->str[0], "cd", 15) == 0)
    	shell->cmds->builtin = &ft_cd;
	if (ft_strncmp(shell->cmds->str[0], "pwd", 15) == 0)
		shell->cmds->builtin = &ft_pwd;
	if (ft_strncmp(shell->cmds->str[0], "export", 15) == 0)
		shell->cmds->builtin = &ft_export;
	if (ft_strncmp(shell->cmds->str[0], "unset", 15) == 0)
		shell->cmds->builtin = &ft_unset;
	if (ft_strncmp(shell->cmds->str[0], "env", 15) == 0)
		shell->cmds->builtin = &ft_env;
	if (ft_strncmp(shell->cmds->str[0], "echo", 15) == 0)
		shell->cmds->builtin = &ft_echo;
	shell->cmds->index = 0;
	shell->cmds->redir = ft_calloc(sizeof(t_redir), 1);
	if (!shell->cmds->redir)
		return (1);
	/*
	shell->cmds->redir->type = l_out;
	shell->cmds->redir->str = ft_strdup("out.txt");
	shell->cmds->redir->next = NULL;
	*/
	shell->cmds->next = NULL;
	return (0);
}

t_shell	*minishell_loop(t_shell *shell)
{
	char			*prompt;
	char			*str;
	t_lexer			*lexer;

	prompt = PROMPT;
	prompt = variable_expansion(prompt, shell);
	str = readline(prompt);
	//add_history(str);
	if (!shell)
		return (NULL);
	lexer = ft_lexer(str);
	//print_lex(lexer);
	shell->cmds = ft_parser(lexer, shell);
	ft_printf("parser returned\n");
	print_simple_cmds_list(shell);
	//add check to exclude empty str in between quotations
	shell->exitcode = expander(shell);
	if (shell->exitcode)
		return (get_error_msg(shell->exitcode), shell);
	shell->exitcode = executor(shell);
	free_lexer(lexer);
	free(prompt);
	free(str);//frees the readline
	free_simple_commands(shell->cmds);
	return (shell);
}

int	main(int ac, char **av, char **envp)
{
	t_shell	*shell;

	shell = ft_calloc(sizeof(t_shell), 1);
	if (!shell)
		return (-1);
	shell->env = arrdup(envp);
	(void)ac;
	(void)av;
	while (1)
	{
		minishell_loop(shell);
		//if exitsignal
		//{
		//	free_tab(shell->env);
		//	free(shell);
		//}
	}
}
