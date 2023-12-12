#ifndef EXECUTOR_H
# define EXECUTOR_H

# define HEREDOC_PROMPT "> "

//executor.c
int 	executor(t_shell *shell);
int		fork_processes(t_shell *shell, pid_t *pid, int **fd);
int		**create_pipes(t_shell *shell, int **fd);
int     execute(t_simple_cmds *cmd, char **envp);

//subprocess.c
int		execute_builtin(t_shell *shell, int **fd, int i);
int	    child_process(t_shell *shell, int	**fd, int i);

//redirections.c
int	redirect_input(t_redir *input);
int	redirect_output(t_redir *output);
int	heredoc(t_redir *heredoc, int index);
int	redirect_fds(int **fd, int i, int amount_of_cmds);
int redirections(t_simple_cmds *cmd);

//utils_exec.c
int     free_and_exit(t_shell *shell, int **fd);
int		count_commands(t_simple_cmds *cmds);
int	    close_unneccesary_fds(int **fd, int i, int amount_of_cmds);
char	*get_path(char *cmd, char **env);
char	*ft_getenv(char *name, char **env);

#endif