#include "minishell.h"

int	count_cmds(t_lexer *lexer)
{
	int	cmds;

	cmds = 1;
	while (lexer != NULL)
	{
		if (lexer->key == l_pipe)
			cmds++;
		lexer = lexer->next;
	}
	printf("count cmds = %i\n", cmds);
	return (cmds);
}

//fills one cmd struct node with data from the lexer, prepares it for executor. returns the index of the last grouped command for the next call to iterate
int	group_commands(int last_cmd, int pipe, t_lexer *lexer, t_simple_cmds *cmd)
{
	cmd->next->prev = cmd;
	while (lexer->index < last_cmd)
		lexer = lexer->next;
	while (lexer->index < pipe)
	{
		if (!lexer->key)
			ft_strlcat(*cmd->str, lexer->token, BUFFER);
		cmd->redir->type = lexer->key;
		cmd->redir->str = lexer->token;
		cmd->amount_of_cmds = count_cmds(lexer);
		lexer = lexer->next;
	}
	return (lexer->index);
	
}
//for debugging 
void print_redir_list(t_redir *head) {
    t_redir *current = head;
    while (current != NULL) {
        printf("Type: %d, String: %s\n", current->type, current->str);
        current = current->next;
    }
}
//for debugghi
void print_simple_cmds_list(t_simple_cmds *head) {
    t_simple_cmds *current = head;
    int i = -1;
    while (current != NULL) {
        ft_printf("Index: %d, Amount of Commands: %d, ", current->index, current->amount_of_cmds);
        while (++i < current->amount_of_cmds)
            ft_printf("String:%s\n", current->str[i]);
        print_redir_list(current->redir); // Print redir list for each t_simple_cmds node
        current = current->next;
    }
}

//counts how many commands will be executed and iterates through lexer to call the grouping function.
t_simple_cmds	*ft_parser(t_lexer *lexer)
{
	int		count;
	int		last_cmd;
	t_lexer		*ptr;
	t_simple_cmds	*cmd;
	t_simple_cmds	*head;

	ptr = lexer;
	count = count_cmds(lexer);
	cmd = (t_simple_cmds *)malloc(sizeof(t_simple_cmds) * count);
	if (!cmd)
		return (NULL);
	cmd->prev = NULL;
	head = cmd;
	while (count > 0)
	{
		if (ptr->key == l_pipe || ptr->next == NULL)
			last_cmd = group_commands(last_cmd, ptr->index, lexer, cmd);
		ptr = ptr->next;
		count--;
	}
	cmd->next = NULL;
	print_simple_cmds_list(head);
	return (head);
}
