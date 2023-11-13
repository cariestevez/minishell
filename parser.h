
#ifndef PARSER_H
# define PARSER_H

# define BUFFER 500
# define PROMPT "Hello \033[36m$USER$ \033[0m"

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

typedef struct s_simple_cmds
{
	char                    **str;
	int                   	(*builtin)(struct s_simple_cmds *);
	struct s_redir			*redir;
	char					**env;
	int						index;
	int						amount_of_cmds;
	struct s_simple_cmds	*next;
	struct s_simple_cmds	*prev;
}	t_simple_cmds;

//lexer.c
t_lexer			*ft_lexer(char *input);
int				free_lexer(t_lexer *lexer);

//parser.c
t_simple_cmds	*ft_parser(t_lexer *lexer);

//expander.c
int 	expander(t_simple_cmds *cmds);
char    *expand_var(char *str, int start, char **env);


//builtins
int		pwd(void);
int 	cd(char *path);

#endif
