#include "minishell.h"

int	redirect_input(t_redir *input)
{
	int 	fd_in;

	if (access(input->str, R_OK) == -1)
		return (ft_printf("minishell: %s: No such file or directory\n", input->str), -1);
	fd_in = open(input->str, O_RDONLY, 0777);
	if (fd_in == -1)
		return (perror("open"), -1);
	if (dup2(fd_in, STDIN_FILENO) == -1)
	{
		close(fd_in);
		perror("dup2");
		return (-1);
	}
	close(fd_in);
	return (0);
}

int	redirect_output(t_redir *output)
{
	int 	fd_out;

	if (output->type == l_append )
		fd_out = open(output->str, O_WRONLY | O_CREAT | O_APPEND, 0777);
	else
		fd_out = open(output->str, O_WRONLY | O_CREAT | O_TRUNC, 0777);
	if (fd_out == -1)
		return (perror("open"), -1);
	if (dup2(fd_out, STDOUT_FILENO) == -1)
	{
		close(fd_out);
		perror("dup2");
		return (-1);
	}
	close(fd_out);
	return (0);
}

int	heredoc(t_redir *heredoc, int index)
{
	char 		*line;
	int		fd;
	char	*temp_file;

	temp_file = ft_strjoin("/tmp/heredoc_temp", ft_itoa(index));
	fd = open(temp_file, O_CREAT | O_RDWR | O_TRUNC, 0644);
	line = readline(HEREDOC_PROMPT);
	while (ft_strncmp(line, heredoc->str, ft_strlen(line)) != 0)
	{
		ft_putstr_fd(line, fd);
		ft_putchar_fd('\n', fd);
		free(line);
		line = readline(HEREDOC_PROMPT);
	}
	free(line);
	heredoc->str = temp_file;
	if (redirect_input(heredoc) != 0)
		return (-1);
	unlink(temp_file);
	free(temp_file);
	return (0);
}

int	redirect_fds(int **fd, int i, int amount_of_cmds)
{
	if (i > 0)
	{
		if (dup2(fd[i - 1][0], 0) < 0)
			return (perror("dup2"), 1);
		close(fd[i][1]);
	}
	if (i < amount_of_cmds - 1)
	{
		if (dup2(fd[i][1], 1) < 0)
			return (perror("dup2"), 1);
		close(fd[i][1]);
	}
	return (0);
}

int redirections(t_simple_cmds *cmd)
{
	t_redir	*head;

	head = cmd->redir;
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
			if (heredoc(cmd->redir, cmd->index) != 0)
				return (-1);
		}
		cmd->redir = cmd->redir->next;
	}
	cmd->redir = head;
	return (0);
}
