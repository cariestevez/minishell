#ifndef EXECUTOR_H
# define EXECUTOR_H

# include "libft.h"
# include "parser.h"
# include <readline/readline.h>
# include <readline/history.h>
# include <errno.h>
# include <sys/types.h>
# include <fcntl.h>
# include <sys/wait.h>

typedef enum
{
    EXECUTOR_SUCCES,
    EXECUTOR_PIPE_ERROR,
    EXECUTOR_FORK_ERROR,
    EXECUTOR_EXEC_ERROR,
    EXECUTOR_REDIRECTION_ERROR,
    EXECUTOR_MALLOC_ERROR,
    EXECUTOR_ACCESS_ERROR,

}   executor_error_code;

//exec.c
int 	executor(t_simple_cmds *cmds);
int		fork_processes(t_simple_cmds *cmds, pid_t *pid, int **fd);
int		**create_pipes(t_simple_cmds *cmds, int **fd);
void     execute(t_simple_cmds *cmds, char **envp, int **fd);

//exec_children.c
int     redirect(t_simple_cmds *cmd, int **fd);
int		execute_builtin(t_simple_cmds *cmds, int **fd, int i);
void	child_process(t_simple_cmds *cmds, int	**fd, int i);

//exec_utils.c
void    free_and_exit(t_simple_cmds *cmds, int **fd, int exitcode);
int		count_commands(t_simple_cmds *cmds);
int		close_unneccesary_fds(int **fd, int i, int amount_of_cmds);
char	*get_path(char *cmd);

//free.c
void	free_tab(char **tab);
void    free_simple_commands(t_simple_cmds *cmds);
void    free_array(int **arr);

void print_simple_cmds_list(t_simple_cmds *head);

//builtins
int		pwd(void);
int 	cd(char *path);

#endif