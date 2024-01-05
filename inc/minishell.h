/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cestevez <cestevez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/15 12:54:21 by emollebr          #+#    #+#             */
/*   Updated: 2024/01/05 10:49:09 by cestevez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "../libft/libft.h"
# include "parser.h"
# include "executor.h"
# include <stdlib.h>
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

extern int	g_last_exit;

//main.c
int		minishell_loop(t_shell *shell);
int		empty_str(char *str);
char	**arrdup(char **env);
char	*get_input(t_shell *shell);

//signals.c
void	signals_non_interactive(void);
void	signals_interactive(void);
void	display_new_line(int signum);
void	reset_rl(int signum);
void	signals_heredoc(void);
void	redisplay_heredoc(int signum);

//free.c
int		free_lexer(t_lexer *lexer);
void	free_char_arr(char **tab);
void	free_int_arr(int **arr);
void	free_cmds(t_simple_cmds *cmd_node);
void	free_on_succes(t_simple_cmds *cmds, t_lexer *lexer, char *prompt);

//builtins
int		ft_exit(t_shell *shell, t_simple_cmds *cmd);
int		ft_pwd(t_shell *shell, t_simple_cmds *cmd);
int		ft_cd(t_shell *shell, t_simple_cmds *cmd);
int		update_envvar(char *name, char *new, char **env);
int		ft_export(t_shell *shell, t_simple_cmds *cmd);
int		ft_echo(t_shell *shell, t_simple_cmds *cmd);
int		ft_env(t_shell *shell, t_simple_cmds *cmd);
int		ft_unset(t_shell *shell, t_simple_cmds *cmd);

#endif
