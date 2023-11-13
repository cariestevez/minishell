#include "minishell.h"

int	redirect_input(t_redir *input)
{
	int 	fd_in;

	fd_in = open(input->str, O_RDONLY, 0777);
	if (fd_in == -1)
	{
		close(fd_in);
		ft_printf("failed at opening file\n");
		return (EXECUTOR_REDIRECTION_ERROR);
	}	
	if (dup2(fd_in, STDIN_FILENO) == -1)
	{
		close(fd_in);
		ft_printf("failed at duplicating fd\n");
		return (EXECUTOR_REDIRECTION_ERROR);
	}			
	close(fd_in);
	return (EXECUTOR_SUCCES);
}

int	redirect_output(t_redir *output)
{
	int 	fd_out;

	if (output->type == l_append )
		fd_out = open(output->str, O_WRONLY | O_CREAT | O_APPEND, 0777);
	else
		fd_out = open(output->str, O_WRONLY | O_CREAT | O_TRUNC, 0777);
	if (fd_out == -1)
	{
		close(fd_out);
		return (EXECUTOR_REDIRECTION_ERROR);
	}	
	if (dup2(fd_out, STDOUT_FILENO) == -1)
	{
		close(fd_out);
		return (EXECUTOR_REDIRECTION_ERROR);
	}			
	close(fd_out);
	return (EXECUTOR_SUCCES);
}

int redirect(t_simple_cmds *cmd, int **fd)
{

	while (cmd->redir != NULL)
	{
		if (cmd->redir->type == l_in && cmd->redir->str != NULL)
		{
			if (redirect_input(cmd->redir) != EXECUTOR_SUCCES)
			{
				ft_printf("child %d failed at redirecting input %s\n", cmd->index, cmd->redir->str);
				free_and_exit(cmd, fd, EXECUTOR_REDIRECTION_ERROR);
			}
		}
		if ((cmd->redir->type == l_append || cmd->redir->type == l_out) 
				&& cmd->redir->str != NULL)
		{
			if (redirect_output(cmd->redir) != EXECUTOR_SUCCES)
			{
				ft_printf("child %d failed at redirecting output %s\n", cmd->index, cmd->redir->str);
				free_and_exit(cmd, fd, EXECUTOR_REDIRECTION_ERROR);
			}
		}
		cmd->redir = cmd->redir->next;
	}
	return (EXECUTOR_SUCCES);
}

int	execute_builtin(t_simple_cmds *cmds, int **fd, int i)
{
	int	std_in;
	int	std_out;

    //store standard streams for restoration later
	std_in = dup(STDIN_FILENO);
	std_out = dup(STDOUT_FILENO);
	if (redirect(cmds, fd) != 0)
		free_and_exit(cmds, fd, EXECUTOR_REDIRECTION_ERROR);
	//redirect in/out to relevant pipes if not first or last command
	if (i > 0)
		if (dup2(fd[i - 1][0], 0) == -1)
			free_and_exit(cmds, fd, EXECUTOR_REDIRECTION_ERROR);
	if (i < cmds->amount_of_cmds - 1)
		if (dup2(fd[i][1], 1) == -1)
			free_and_exit(cmds, fd, EXECUTOR_REDIRECTION_ERROR);
	//execute the builtin function
	cmds->builtin(cmds);
	//restore standard streams because this is the parent process
    if (!dup2(std_in, STDIN_FILENO) || !dup2(std_out, STDOUT_FILENO))
        free_and_exit(cmds, fd, EXECUTOR_REDIRECTION_ERROR);
    return (0);
}

void	child_process(t_simple_cmds *cmds, int	**fd, int i)
{
	close_unneccesary_fds(fd, i, cmds->amount_of_cmds);
    //if process is first command, input is redirected from infile or stay as stin
	//if process is last command, output is redirected to outfile or stay as stdout
	if (redirect(cmds, fd) != 0)
		free_and_exit(cmds, fd, EXECUTOR_REDIRECTION_ERROR);
	//replace stdin with read end of pipe i-1 if not
	if (cmds->index != 0)
	{
		if(dup2(fd[i - 1][0], 0) == -1)
		{
			perror("Error duplicating stdin");
            free_and_exit(cmds, fd, EXECUTOR_REDIRECTION_ERROR);
		}
	}
	//replace stdout with write part of pipe i if not
	if (cmds->index != cmds->amount_of_cmds - 1)
	{
		if(dup2(fd[i][1], 1) == -1)
         {
			perror("Error duplicating stdout");
            free_and_exit(cmds, fd, EXECUTOR_REDIRECTION_ERROR);
		}
	}
	//close remaining fds
	if (i != 0)
		close(fd[i - 1][0]);
	if (i != cmds->amount_of_cmds - 1)
		close(fd[i][1]);
	//execute command - on success, function never returns here
	execute(cmds, cmds->env, fd);
}