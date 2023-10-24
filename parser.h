
#ifndef PARSER_H
# define PARSER_H

# include "libft.h"
# include <readline/readline.h>
# include <readline/history.h>

# ifndef NULL
#  define NULL ((void*)0)
# endif
# define BUFFER 500

typedef enum e_lexertype
{
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
	l_space
}	t_lexertype;

typedef struct s_lexer
{
	char    		*str;
	t_lexertype     key;
	int				index;
	struct s_lexer	*next;
	struct s_lexer	*prev;
}	t_lexer;

typedef struct s_simple_cmds
{
	char                    **str;
	//int                     (*builtin)(t_tools *, struct s_simple_cmds *);
	t_lexer					*in;
	t_lexer					*out;
	struct s_simple_cmds	*next;
	struct s_simple_cmds	*prev;
}	t_simple_cmds;

t_lexer			*ft_lexer(char *input);
int				free_lexer(t_lexer *lexer);
t_simple_cmds	*ft_parser(t_lexer *lexer);
int				pwd(void);
int 	cd(char *path);

#endif
