#include "minishell.h"

//-------------------------------------------------------------------------------------------

// if token bewtween parentheses
// 	-include extra step to separe what is inside into independet tokens
// if env_variable
// 	-expand inmediately before executing (in bash it is expanded right after tokenization/lexer)
// if quotes (see cases)
// 	-remove in expander

// if 1st node is pipe or no command before or after pipe (but another redir or end of str)
// 		syntax error
// if | everything before it gets executed and piped to the command after it
// if < takes the FILE after(on its right) as input and redirects it to the 1st command inmediately before the redir(on its left)
//		if < at the beginning of cmd line, cmd is the 1st one after the FILE
// if > takes the output of the last simple command before the operand, that gets redirected to the FILE after it(on its right)
//		if > at the beginnig of the cmd line, cmd is the 1st one after the FILE
// if some of the commands cannot be executed, it just creates an empty file with the specified name

//shell->cmd->str => (**str)-> 1st ptr to str: name of executable(cmd), n-ptr(s): option(s) to the cmd, last ptr: NULL terminator str


//if lexer->idx 1 then search and save until pipe. else search and save until pipe or lexer = NULL

//make it return t_lexer * (also when done with saving tokens, return head or something different than NULL
//bc returning NULL is used as indicator that something went wrong in the saving process)

int	count_tokens(t_lexer *lexer, t_shell *shell)
{
	int	cmd_tokens;
	int	redir_tokens;

	cmd_tokens = 0;
	redir_tokens = 0;
	while (lexer->token != NULL && lexer->key != l_pipe)//count total ammount of tokens and redirections
	{
		if (lexer->key == l_in || lexer->key == l_out
			|| lexer->key == l_append || lexer->key == l_heredoc)
			redir_tokens++;
		cmd_tokens++;
		lexer = lexer->next;
	}
	cmd_tokens -= (redir_tokens * 2);//substract redir operators and for each of them a file or delimiter
	shell->cmds->str = (char **)malloc(sizeof(char *) * (cmd_tokens + 1));
	if (!shell->cmds->str)
		return (-1);
	return (cmd_tokens);
}

t_lexer*	save_simple_cmd(t_lexer	*lexer, t_shell	*shell)
{
	int		i;
	int		cmd_tokens;
	t_lexer	*head;

	i = 0;
	head = lexer;
	cmd_tokens = count_tokens(lexer, shell);
	lexer = head;
	while (lexer->token != NULL && lexer->key != l_pipe)//saves the redirections in order of appearance
	{
		if (lexer->key == l_in || lexer->key == l_out
			|| lexer->key == l_append || lexer->key == l_heredoc)
		{
			if (!lexer->next)//ERROOOR no file name after redir -> bash retuns syntax error
				return (NULL);
			shell->cmds->redir = new_redir_node(lexer->next->token, lexer->key);
			shell->cmds->redir = shell->cmds->redir->next;
		}
		else if (lexer->key == l_non_op)//if cmd found, saves it and so the options next to it
		{
			while (cmd_tokens > 0) //(lexer != NULL && shell->cmds->str != NULL)
			{
				//ft_printf("cmd->token; %s\n", shell->cmds->);
				//shell->cmds->str[i] = (char *)malloc(sizeof(char) * (ft_strlen(lexer->token) + 1));
				//if (!shell->cmds->str[i])
					//return (NULL);
				shell->cmds->str[i] = ft_strdup(lexer->token);
				//if (!shell->cmds->str[i])
					//return (NULL);	
				ft_printf("in save simple cmds, the current token is %s in lexer, saved as %s\n", lexer->token, shell->cmds->str[i]);
				i++;
				cmd_tokens--;
			}
			lexer = lexer->next->next;
			//shell->cmds->str[i] = ft_calloc(1, sizeof(char));
			//if (shell->cmds->str[i] == NULL)
			// 	//return (NULL);
		}
	}
	if (!lexer)
	{
		ft_printf("segfault check, lexer is null\n");
		lexer = head;
	}
	return (lexer);
}

//if error, return to main before exiting to finish to free shell//free shell->cmds to indicate ERROR to the calling func
t_simple_cmds	*ft_parser(t_lexer *lexer, t_shell *shell)
{
	int				cmd;
	t_simple_cmds	*head_cmd;

	cmd = 0;
	head_cmd = NULL;
	shell->amount_of_cmds = count_cmds(lexer);
	if (shell->amount_of_cmds == 0)//in case of syntax error (pipe on 1st position)
		return (NULL);//returns without alloc shell->cmdss (indicator for failure in main)
	shell->cmds = new_cmd_node(NULL);
	if (!shell->cmds)//current cmd node
	{
		free_cmds(head_cmd);//frees previous shell->cmds saved in the loop in case
		//head_cmd = NULL;
		return (NULL);
	}
	while (cmd < shell->amount_of_cmds) //each loop saves 1 cmd
	{
		ft_printf("command number %d\n", shell->cmds->index);
		if (shell->cmds->prev == NULL)
			head_cmd = shell->cmds;//saves ptr to 1st command for freeing if needed
		ft_printf("segfault check 1\n");
		lexer = save_simple_cmd(lexer, shell); //THIS IS THE BUG!! IT SEGFAULTS ON THE LAST COMMAND.
		ft_printf("segfault check 2\n");
		if (lexer == NULL && cmd < shell->amount_of_cmds - 1)//indicator for error in save_cmd 
		{
			//ERROR saving a cmd
			free_cmds(head_cmd);
			return (NULL);
		}
		print_simple_cmds_list(shell);
		shell->cmds->next = new_cmd_node(shell->cmds);
		if (!shell->cmds->next)
		{
			free_cmds(head_cmd);//frees previous shell->cmds saved in the loop in case
			//head_cmd = NULL;
			return (NULL);
		}
		shell->cmds = shell->cmds->next;
		cmd++;
	}
	add_builtin_ptr(head_cmd);
	return (head_cmd);
}

void	add_builtin_ptr(t_simple_cmds *cmd)
{
	t_simple_cmds *head;

	head = cmd;
	while (cmd != NULL)
	{
		if (ft_strncmp(cmd->str[0], "cd", 2))
			cmd->builtin = &ft_cd;
		else if (ft_strncmp(cmd->str[0], "echo", BUFFER))
			cmd->builtin = &ft_echo;
		else if (ft_strncmp(cmd->str[0], "env", BUFFER))
			cmd->builtin = &ft_env;
		else if (ft_strncmp(cmd->str[0], "export", BUFFER))
			cmd->builtin = &ft_export;
		else if (ft_strncmp(cmd->str[0], "pwd", BUFFER))
			cmd->builtin = &ft_pwd;
		else if (ft_strncmp(cmd->str[0], "unset", BUFFER))
			cmd->builtin = &ft_unset;
		//else if (ft_strncmp(cmd->str[0], "exit", 4))
			//cmd->builtin = &ft_exit;
		cmd = cmd->next;
	}
	cmd = head;
	return ;
}



// //returns idx of current token to iterate the linked list
// int	scan_simple_cmd(t_lexer *lexer, int start, t_shell *shell, int cmd_0)
// {
// 	int		i;
// 	int		token_count;
// 	int		redir;
// 	int		num_of_str;
// 	t_lexer	*head;

// 	i = 0;		
// 	token_count = 0;
// 	redir = 0;
// 	num_of_str = 0;
// 	if (shell->amount_of_cmds == 1)//not needed to search for a pipe
// 		return (single_cmd(lexer, shell));
// //SEARCH NEXT PIPE (DELIMIT SIMPLE CMD)
// 	while (lexer != NULL && lexer->index != start)//go to 1st token of current cmd
// 		lexer = lexer->next;
// 	head = lexer;//save position of 1st token
// 	while (lexer != NULL && lexer->key != l_pipe)
// 	{
// 		lexer = lexer->next;
// 		token_count++;
// 	}
// //COUNT NUM OF REDIR IN SIMPLE CMD(typical structure: 1 cmd + x num of redirections + 1 file/delimiter for each of the redir)
// //how many of the tokens are redir->allocate redir struct
// 	lexer = head;//start from 1st token of this cmd again
// 	i = token_count;
// 	while (lexer != NULL && token_count > 0)
// 	{
// 		if (lexer->key == l_in || lexer->key == l_out
// 			|| lexer->key == l_append || lexer->key == l_heredoc)
// 			redir++;
// 		token_count--;
// 		lexer = lexer->next;
// 	}
// //ALLOCATE FOR THE CMD STR ARRAYS
// 	shell->cmd->str = (char **)malloc(sizeof(char *) * (i - (redir * 2) + 1));
// 	//if (shell->cmd->str == NULL)
// 		//ERROOORRRR
// //GO BACK TO THE START
// 	lexer = head;
// 	while (i > 0)//go through all the tokens of this cmd
// 	{
// 		if (lexer->key == l_non_op)
// 			lexer = lexer->next;
// 		else//SAVE REDIR NODE

// 		i--;
// 	}
// 	return (start);
// }

// t_lexer	scan_simple_cmd(t_lexer *lexer, t_shell *shell, int cmd_0)//always starting at the head of lexer list
// {
// 	if (shell->amount_of_cmds == 1)
// 		return(save_simple_cmd(lexer, 0, shell));
// 	if (lexer->index == 0)
// 		shell->cmd->before_pipe
// }