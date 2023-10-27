#include "parser.h"

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
		if (lexer->key == NULL)
			ft_strlcat(*cmd->str, lexer->str, BUFFER);
		if (lexer->key == l_out)
			cmd->out = lexer->str;
		else if (lexer->key == l_in)
			cmd->in = lexer->str;
		lexer = lexer->next;
	}
	return (lexer->index);
	
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
	head = cmd;
	cmd->prev = NULL;
	while (count > 0)
	{
		if (*ptr->key == l_pipe || ptr->next == NULL)
			last_cmd = group_commands(last_cmd, ptr->index, lexer, cmd);
		ptr = ptr->next;
		count--;
	}
	cmd->next = NULL;
	cmd = head;
	return (head);

}
