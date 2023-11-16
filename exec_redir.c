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
	return (SUCCESS);
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
	return (SUCCESS);
}

int	heredoc(t_redir *heredoc)
{
	char 		*line;
	int				fd;
	char	*temp_file;

	temp_file = "heredoc_temp";
	fd = open(temp_file, O_CREAT | O_RDWR | O_TRUNC, 0644);
	line = readline(HEREDOC_PROMPT);
	//heredoc->str is the delimiting identifier of the heredoc
	while (ft_strnstr(line, heredoc->str, ft_strlen(line)))
	{
		ft_putstr_fd(line, fd);
		ft_putchar_fd('\n', fd);
		free(line);
		line = readline(HEREDOC_PROMPT);
	}
	free(line);
	heredoc->str = temp_file;
	if (redirect_input(heredoc) != 0)
		return (EXECUTOR_HEREDOC_ERROR);
	unlink(temp_file);
	//temp_file will be deleted once all fds are closed
	return (SUCCESS);
}

int redirections(t_simple_cmds *cmd)
{
	while (cmd->redir != NULL)
	{
		if (cmd->redir->type == l_in && cmd->redir->str != NULL)
		{
			if (redirect_input(cmd->redir) != 0)
				return (-1);
		}
		if ((cmd->redir->type == l_append || cmd->redir->type == l_out)
				&& cmd->redir->str != NULL)
		{
			if (redirect_output(cmd->redir) != 0)
				return (-1);
		}
		if (cmd->redir->type == l_heredoc && cmd->redir->str != NULL)
		{
			if (heredoc(cmd->redir) != 0)
				return (-1);
		}
		ft_printf("child %d redirected to %s\n", cmd->redir->str);
		cmd->redir = cmd->redir->next;
	}
	return (SUCCESS);
}
