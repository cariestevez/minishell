#ifndef MINISHELL_H
# define MINISHELL_H

# include "./libft/libft.h"
# include "parser.h"
# include "executor.h"
# include <readline/readline.h>
# include <readline/history.h>
# include <errno.h>
# include <sys/types.h>
# include <fcntl.h>
# include <sys/wait.h>
# include <errno.h>

//free.c
int		free_lexer(t_lexer *lexer);
void	free_tab(char **tab);
void    free_simple_commands(t_simple_cmds *cmds);
void    free_array(int **arr);

//builtins
int		ft_pwd(t_shell *shell);
int 	ft_cd(t_shell *shell);
int		ft_echo(t_shell *shell);
int		ft_env(t_shell *shell);
int		ft_unset(t_shell *shell);
int		ft_export(t_shell *shell);

#endif