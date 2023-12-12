
#ifndef PARSER_H
# define PARSER_H

# include "../libft/libft.h"
# include <readline/readline.h>
# include <readline/history.h>

# ifndef NULL
#  define NULL ((void*)0)
# endif
# define BUFFER 500

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
	char    		*token;
	t_lexertype     key;
	int				index;
	struct s_lexer	*next;
	struct s_lexer	*prev;
}	t_lexer;

typedef struct s_redir
{
	t_lexertype		type;
	char			*str;
	struct s_redir	*next;
}	t_redir;

typedef struct	s_shell
{
	struct s_simple_cmds	*cmds;
	int				amount_of_cmds;
	char					**env;
	int					exitcode;
} t_shell;

typedef   int (*builtin_func)(struct s_shell *, struct s_simple_cmds *);

typedef struct s_simple_cmds
{
	char                    **str;
	int						index;
	builtin_func            builtin;
	struct s_redir			*redir;
	struct s_simple_cmds	*next;
	struct s_simple_cmds	*prev;
}	t_simple_cmds;

//lexer.c
t_lexer		*ft_lexer(char *input);
int			read_command_line(t_lexer *lexer, char *str);
int			save_token(t_lexer *lexer, char *str, int start, int len);
t_lexertype	get_key(char *str);

//utils_lexer.c
t_lexer 	*new_lexnode(t_lexer *prev, int index);
int			open_quotes(char *str);
int			open_brackets(char *str);
int			open_curly(char *str);

//parser.c
t_simple_cmds	*ft_parser(t_lexer *lexer, t_shell *shell);
int				save_simple_cmd(t_lexer	*lexer, t_shell	*shell);
t_redir			*save_redirection(t_shell *shell, t_lexer *lexer, int redir_count);
int				count_tokens(t_lexer *lexer, t_shell *shell);
void			add_builtin_ptr(t_simple_cmds *cmd);

//utils_parser.c
t_simple_cmds	*new_cmd_node(t_simple_cmds *prev);
void	free_cmds(t_simple_cmds *cmd_node);
int		count_cmds(t_lexer	*lexer);
t_redir *new_redir_node(char *file, t_lexertype type);

//expander.c
int 	expander(t_shell *shell);
char	*check_for_variables(char *str, t_shell *shell);
int		get_expanded_variable(char **str, int start, int end, t_shell *shell);

//utils_expander.c
int		declare_variable(char *var, t_shell *shell);
int 	quote_removal(t_shell *shell, int i);


//debug.c
void print_lex(t_lexer *lexer);
void print_simple_cmds_list(t_shell *shell);
void print_redir_list(t_redir *head);
//void	print_cmds(t_simple_cmds *cmds);

#endif
