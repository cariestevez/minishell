/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emollebr <emollebr@student.42berlin.d      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/13 22:10:27 by emollebr          #+#    #+#             */
/*   Updated: 2023/12/13 22:10:28 by emollebr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXECUTOR_H
# define EXECUTOR_H

# define HEREDOC_PROMPT "> "

//executor.c
int		executor(t_shell *shell);
int		fork_processes(t_shell *shell, pid_t *pid, int **fd);
int		**create_pipes(t_shell *shell, int **fd);
int		pipe_fds(int **fd, int amount_of_cmds);
void	handle_fork_error(int **fd, int i);

//subprocess.c
int		execute(t_simple_cmds *cmd, char **envp);
int		execute_builtin(t_shell *shell, int **fd, int i);
int		child_process(t_shell *shell, int	**fd, int i);
int		wait_for_children(t_simple_cmds *head, pid_t *pid, int amount_of_cmds);
int		redirections(t_simple_cmds *cmd);

//redirections.c
int		redirect_input(t_redir *input);
int		redirect_output(t_redir *output);
int		heredoc(t_redir *heredoc, int index);
int		redirect_fds(int **fd, int i, int amount_of_cmds);

//utils_exec.c
int		free_and_exit(t_shell *shell, int **fd, pid_t *pid);
int		count_commands(t_simple_cmds *cmds);
int		close_unneccesary_fds(int **fd, int i, int amount_of_cmds);
char	*get_path(char *cmd, char **env);
char	*ft_getenv(char *name, char **env);

#endif
