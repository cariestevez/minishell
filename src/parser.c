#include "minishell.h"

//-------------------------------------------------------------------------------------------


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


//counts the total ammount of tokens and the redirection tokens before finding a pipe
//allocates memory for the command array
//returns error if allocation fails, if there are more redir than cmd tokens or if token is a pipe(in case of two consecutive pipes)
int	count_tokens(t_lexer *lexer, t_shell *shell)
{
	int	cmd_tokens;
	int	redir_tokens;

	cmd_tokens = 0;
	redir_tokens = 0;
	while (lexer != NULL && lexer->token != NULL && lexer->key != l_pipe)
	{
		if (lexer->key == l_in || lexer->key == l_out || lexer->key == l_append || lexer->key == l_heredoc)
			redir_tokens++;
		cmd_tokens++;
		lexer = lexer->next;
	}
	cmd_tokens -= (redir_tokens * 2);
	ft_printf("cmds = %d redirections = %d\n", cmd_tokens, redir_tokens);
	if (cmd_tokens <= 0 && redir_tokens != 0)
		return (0);
	if (cmd_tokens <= 0)
		return (-1);
	shell->cmds->str = (char **)malloc(sizeof(char *) * (cmd_tokens + 1));
	if (shell->cmds->str == NULL)
		return (-1);
	ft_printf("allocated for %d + 1 cmds\n", cmd_tokens);
	return (cmd_tokens);
}

t_redir	*save_redirection(t_shell *shell, t_lexer *lexer, int redir_count)
{
	if (redir_count == 1)
	{
		shell->cmds->redir = new_redir_node(lexer->next->token, lexer->key);
		if (shell->cmds->redir == NULL)
			return (NULL);
	}
	else
	{
		shell->cmds->redir->next = new_redir_node(lexer->next->token, lexer->key);
		if (shell->cmds->redir == NULL)
			return (NULL);
		shell->cmds->redir = shell->cmds->redir->next;
	}
	return (shell->cmds->redir);
}

int	save_simple_cmd(t_lexer	*lexer, t_shell	*shell)
{
	int		i;
	int		cmd_tokens;
	int		redir_count;
	t_lexer	*head;
	t_redir	*redir_head;

	i = 0;
	redir_head = NULL;
	redir_count = 0;
	head = lexer;
	cmd_tokens = count_tokens(lexer, shell);
	if (cmd_tokens < 0)
		return (-1);
	lexer = head;
	while (lexer != NULL && lexer->token != NULL && lexer->key != l_pipe)
	{
		if (lexer->key == l_in || lexer->key == l_out
			|| lexer->key == l_append || lexer->key == l_heredoc)
		{
			redir_count++;
			ft_printf("found a redir token  in save simple_cmd\n");
			shell->cmds->redir = save_redirection(shell, lexer, redir_count);
			if (shell->cmds->redir == NULL)
				return (-1);
			if (redir_count == 1)
				redir_head = shell->cmds->redir;
			// if (lexer->next != NULL)
			lexer = lexer->next->next;
			// else
			// 	lexer = NULL;
		}
		else if (lexer->key == l_non_op)
		{
			while (cmd_tokens > 0 && lexer && lexer->key == l_non_op)
			{
				shell->cmds->str[i] = ft_strdup(lexer->token);
				i++;
				cmd_tokens--;
				lexer = lexer->next;
			}
			shell->cmds->str[i] = NULL;
		}
	}
	shell->cmds->redir = redir_head;
	if (lexer && lexer->key == l_pipe)
		lexer = lexer->next;
	if (lexer == NULL)
		return (-1);
	return (lexer->index);
}

//if error, return NULL to main (free shell->cmds first if allocated already)
t_simple_cmds	*ft_parser(t_lexer *lexer, t_shell *shell)
{
	int				cmd;
	int				idx;
	t_simple_cmds	*head_cmd;

	idx = 0;
	cmd = 0;
	shell->amount_of_cmds = count_cmds(lexer);
	if (shell->amount_of_cmds == 0)
		return (NULL);
	shell->cmds = new_cmd_node(NULL);
	head_cmd = shell->cmds;
	if (shell->cmds == NULL)
		return (NULL);
	while (cmd < shell->amount_of_cmds)
	{
		idx = save_simple_cmd(lexer, shell);//saves until the pipe and returns the idx of the position after
		while (idx > 0 && lexer && lexer->index != idx)//we move the lexer after the pipe
			lexer = lexer->next;
		if (lexer == NULL)//this shouldn't be necessary if we handle the case of pipe on last position in count_cmds
		{
			free_cmds(head_cmd);
			head_cmd = NULL;
			return (NULL);
		}
		if (cmd < shell->amount_of_cmds - 1)
		{
			shell->cmds->next = new_cmd_node(shell->cmds);
			if (shell->cmds->next == NULL)
			{
				free_cmds(head_cmd);
				head_cmd = NULL;
				return (NULL);
			}
			shell->cmds = shell->cmds->next;
		}
		cmd++;
	}
	print_simple_cmds_list(shell);
	add_builtin_ptr(head_cmd);
	return (head_cmd);
}

void	add_builtin_ptr(t_simple_cmds *cmd)
{
	t_simple_cmds *head;

	head = cmd;
	while (cmd != NULL && cmd->str != NULL)
	{
		if (ft_strncmp(cmd->str[0], "cd", BUFFER) == 0)
			cmd->builtin = &ft_cd;
		else if (ft_strncmp(cmd->str[0], "echo", BUFFER) == 0)
			cmd->builtin = &ft_echo;
		else if (ft_strncmp(cmd->str[0], "env", BUFFER) == 0)
			cmd->builtin = &ft_env;
		else if (ft_strncmp(cmd->str[0], "export", BUFFER) == 0)
			cmd->builtin = &ft_export;
		else if (ft_strncmp(cmd->str[0], "pwd", BUFFER) == 0)
			cmd->builtin = &ft_pwd;
		else if (ft_strncmp(cmd->str[0], "unset", BUFFER) == 0)
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