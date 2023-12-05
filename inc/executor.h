#ifndef EXECUTOR_H
# define EXECUTOR_H

# define HEREDOC_PROMPT "heredoc> "

//executor.c
int 	executor(t_shell *shell);
int		fork_processes(t_shell *shell, pid_t *pid, int **fd);
int		**create_pipes(t_shell *shell, int **fd);
int     execute(t_simple_cmds *cmd, char **envp);

//exec_children.c
int		execute_builtin(t_shell *shell, int **fd, int i);
void	child_process(t_shell *shell, int	**fd, int i);

//exec_redir.c
int	redirect_input(t_redir *input);
int	redirect_output(t_redir *output);
int redirections(t_simple_cmds *cmd);

//exec_utils.c
int    free_and_exit(t_shell *shell, int **fd, int exitcode);
int		count_commands(t_simple_cmds *cmds);
int	    close_unneccesary_fds(int **fd, int i, int amount_of_cmds);
char	*get_path(char *cmd, char **env);

void print_simple_cmds_list(t_shell *shell);

//builtins
int		pwd(void);
int 	cd(char *path);

#endif