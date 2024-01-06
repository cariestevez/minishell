/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cestevez <cestevez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/05 09:38:15 by emollebr          #+#    #+#             */
/*   Updated: 2024/01/06 21:37:17 by cestevez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#define PARSER_H

#include "../libft/libft.h"
#include <readline/readline.h>
#include <readline/history.h>

#define BUFFER 500

typedef enum e_lexertype
{
	l_nothing,
	l_non_op,
	l_in,
	l_out,
	l_append,
	l_heredoc,
	l_pipe,
}	t_lexertype;

typedef struct s_lexer
{
	int				index;
	t_lexertype		key;
	char			*token;
	struct s_lexer	*next;
	struct s_lexer	*prev;
}	t_lexer;

typedef struct s_redir
{
	t_lexertype		type;
	char			*str;
	struct s_redir	*next;
	struct s_redir	*head;
}	t_redir;

typedef struct s_shell
{
	int							amount_of_cmds;
	int							exitcode;
	struct s_simple_cmds		*cmds;
	struct s_simple_cmds		*cmd_head;
	char						**env;
	int							std[2];
}	t_shell;

typedef int	(*t_builtin_func)(struct s_shell *, struct s_simple_cmds *);

typedef struct s_simple_cmds
{
	int						index;
	t_builtin_func			builtin;
	int						num_strs;
	char					**str;
	struct s_redir			*redir;
	struct s_redir			*redir_head;
	struct s_simple_cmds	*next;
	struct s_simple_cmds	*prev;
}	t_simple_cmds;

//lexer.c
int				save_token(t_lexer *lexer, char *str, int start, int len);
int				tokenize_redir(t_lexer *lexer, char *str, int start, int i);
int				tokenize_cmd(t_lexer *lexer, char *str, int start, int i);
int				read_command_line(t_lexer *lexer, char *str);
t_lexer			*ft_lexer(char *str);

//lexer_utils.c
t_lexer			*new_lexnode(t_lexer *prev, int index);
int				open_quotes(char *str);
int				look_for(int bracket, char *str, int i);
int				open_brackets(char *str);
int				open_curly(char *str);

//lexer_utils2.c
int				skip_spaces(char *str, int i);
t_lexertype		get_key(char *str);

//parser.c
t_simple_cmds	*ft_parser(t_lexer *lexer, t_shell *shell);
int				parser_loop(t_shell *shell, t_lexer *lexer);
int				save_simple_cmd(t_lexer	*lexer, t_shell	*shell);
t_redir			*save_redirection(t_shell *shell, t_lexer *lexer,
					int redir_count);
t_lexer			*save_cmd_str(t_shell *shell, t_lexer *lexer,
					int *t_and_r, int i);

//parser_utils.c
int				count_cmds(t_lexer	*lexer);
int				count_tokens(t_lexer *lexer);
void			add_builtin_ptr(t_simple_cmds *cmd);
t_redir			*new_redir_node(char *file, t_lexertype type);
t_simple_cmds	*new_cmd_node(t_simple_cmds *prev);

//expander.c
int				expander(t_shell *shell);
int				get_expanded_variable(char **str, int start,
					int end, t_shell *shell);
int				get_expanded_exitcode(char **str, int start,
					int end);
int				get_quotes_trimmed(char **str, int start, int end);
char			*check_for_variables(char *str, t_shell *shell);

//expander_utils.c
int				declare_variable(char *var, t_shell *shell);
int				if_quotes(char **str, int *q_flag, int i);
int				if_variable(char **str, t_shell *shell, int i);
int				add_variable(char *var, t_shell *shell, int i);
int				replace_variable(char *var, char *new_var,
					t_shell *shell, int i);

//utils_random.c
int				restore_streams(int *std);
int				check_edgecases(t_shell *shell);
void			remove_quotes(t_shell *shell);
