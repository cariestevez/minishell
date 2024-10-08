#ifndef MINISHELL_H
# define MINISHELL_H

# include "../libft/libft.h"
# include "parser.h"
# include "executor.h"
# include <readline/readline.h>
# include <readline/history.h>
# include <signal.h>
# include <sys/ioctl.h>
# include <sys/types.h>
# include <fcntl.h>
# include <sys/wait.h>
# include <errno.h>

# define BUFFER 500
# define PROMPT "\033[35mHello \033[36m${USER}\033[0m$ "

static volatile sig_atomic_t handle_me = 0;

//main.c
t_shell	*minishell_loop(t_shell *shell);
char    **arrdup(char **env);
void	free_on_succes(t_simple_cmds *cmds, t_lexer *lexer, char *prompt);

//free.c
void	free_char_arr(char **tab);
void    free_int_arr(int **arr);
int		free_lexer(t_lexer *lexer);
void    free_simple_commands(t_simple_cmds *cmds);

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
