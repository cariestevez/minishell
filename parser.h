
#ifndef PARSER_H
# define PARSER_H

typedef enum e_lexertype
{
	l_non_op,
	l_space,
	l_quote,
	l_dquote,
	l_word,
	l_asterisk,
	l_paraopen,
	l_paraclose,
	l_in,
	l_out,
	l_append,
	l_pipe,
	l_heredoc,
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
	//if type == l_heredoc, str is the delimiting identifier
	char			*str;
	struct s_redir	*next;
}	t_redir;

typedef struct	s_shell
{
	struct s_simple_cmds	*cmds;
	int				amount_of_cmds;
	char					**env;
	char				**locvars;
} t_shell;

typedef struct s_simple_cmds
{
	char                    **str;
	int                   	(*builtin)(struct s_shell *, struct s_simple_cmds *);
	struct s_redir			*redir;
	int						index;
	struct s_simple_cmds	*next;
	struct s_simple_cmds	*prev;
}	t_simple_cmds;

//lexer.c
t_lexer			*ft_lexer(char *input);

//parser.c
t_simple_cmds	*ft_parser(t_lexer *lexer);

//expander.c
int 	expander(t_shell *shell);
int		declare_variable(char *var, char **locvars);
char	*variable_expansion(char *str, char **env);
char	*replace_variable(char *str, int start, int end, char **env);
char	*ft_getenv(char *name, char **env);


#endif
