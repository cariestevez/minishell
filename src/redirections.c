/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emollebr <emollebr@student.42berlin.d      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/13 22:03:27 by emollebr          #+#    #+#             */
/*   Updated: 2023/12/13 22:03:29 by emollebr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	redirect_input(t_redir *input)
{
	int	fd_in;

	if (access(input->str, R_OK) == -1)
	{
		ft_printf("minishell: %s: No such file or directory\n", input->str);
		return (-1);
	}
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
	int	fd_out;

	if (output->type == l_append)
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

//ctrl C prints C in same line and gives back prompt
//ctrl D gives back the prompt
//ctrl \ does NOTHING
static void	read_heredoc(char *delimiter, int fd)
{
	char	*line;

	signals_heredoc();
	while (g_last_exit != 130)
	{
		write(1, HEREDOC_PROMPT, ft_strlen(HEREDOC_PROMPT));
		line = get_next_line(STDIN_FILENO);
		if (!line)
			break;
		printf("g_last_extit: %i\n", g_last_exit);
		if (ft_strncmp(line, delimiter, ft_strlen(delimiter)) == 0)
			break;
		ft_putstr_fd(line, fd);
		free(line);
	}
	printf("we broke free\n");
	rl_redisplay();
	signals_non_interactive();
	if (g_last_exit != 130)
		free(line);
}

int	heredoc(t_redir *heredoc, int index)
{
	int		fd;
	char	*temp_file;
	char	*file_num;

	file_num = ft_itoa(index);
	temp_file = ft_strjoin("/tmp/heredoc_temp", file_num);
	free(file_num);
	fd = open(temp_file, O_CREAT | O_RDWR | O_TRUNC, 0644);
	signals_interactive();
	read_heredoc(heredoc->str, fd);
	free(heredoc->str);
	heredoc->str = temp_file;
	if (redirect_input(heredoc) != 0)
		return (-1);
	unlink(temp_file);
	return (0);
}

int	redirect_fds(int **fd, int i, int amount_of_cmds)
{
	if (i > 0)
	{
		if (dup2(fd[i - 1][0], 0) < 0)
			return (perror("dup2"), 1);
		close(fd[i - 1][1]);
	}
	if (i < amount_of_cmds - 1)
	{
		if (dup2(fd[i][1], 1) < 0)
			return (perror("dup2"), 1);
		close(fd[i][1]);
	}
	return (0);
}
