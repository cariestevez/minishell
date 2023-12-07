#ifndef MINISHELL_H
# define MINISHELL_H

# include "../libft/libft.h"
# include "parser.h"
# include "executor.h"
# include <readline/readline.h>
# include <readline/history.h>
# include <errno.h>
# include <sys/types.h>
# include <fcntl.h>
# include <sys/wait.h>
# include <errno.h>

# define BUFFER 500
# define PROMPT "\033[35mHello \033[36m${USER}\033[0m$ "
# define WELCOME "welcome.txt"

typedef enum    e_exitcode
{
    SUCCESS,
    DECLARE_VAR_ERROR,
    EXPANDER_VAR_ERROR,
    EXECUTOR_PIPE_ERROR,
    EXECUTOR_FORK_ERROR,
    EXECUTOR_EXEC_ERROR,
    EXECUTOR_REDIRECTION_ERROR,
    EXECUTOR_MALLOC_ERROR,
    EXECUTOR_ACCESS_ERROR,
    EXECUTOR_HEREDOC_ERROR,
    STDSTREAM_RESTORE_ERROR,
}   exitcode;

//main.c
t_shell	*minishell_loop(t_shell *shell, char *prompt);
char    **arrdup(char **env);
void	free_on_succes(t_simple_cmds *cmds, t_lexer *lexer, char *prompt);

//free.c
int		free_lexer(t_lexer *lexer);
void	free_tab(char **tab);
void    free_simple_commands(t_simple_cmds *cmds);
void    free_i_array(int **arr);
void	get_error_msg(int exitcode);

//builtins
int		ft_pwd(t_shell *shell, t_simple_cmds *cmd);
int 	ft_cd(t_shell *shell, t_simple_cmds *cmd);
int     update_envvar(char *name, char *new, char **env);
int     ft_export(t_shell *shell, t_simple_cmds *cmd);
int		ft_echo(t_shell *shell, t_simple_cmds *cmd);
int		ft_env(t_shell *shell, t_simple_cmds *cmd);
int		ft_unset(t_shell *shell, t_simple_cmds *cmd);
int		ft_export(t_shell *shell, t_simple_cmds *cmd);

#endif
