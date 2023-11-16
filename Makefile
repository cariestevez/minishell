################################################################################
NAME := minishell
CFLAGS := -g -Wall -Wextra -Werror -I.
LIBFTFLAGS := -Ilibft -Llibft -lreadline
LIBFT := ./libft/
LIBS := $(LIBFT)libft.a
BUILTINS := ./builtins/
SRCS := $(BUILTINS)ft_cd.c \
		$(BUILTINS)ft_pwd.c \
		$(BUILTINS)ft_echo.c \
		$(BUILTINS)ft_env.c \
		$(BUILTINS)ft_unset.c \
		free.c \
		exec.c \
		exec_children.c \
		exec_redir.c \
		exec_utils.c \
		main.c \
		expander.c \
#		lexer.c \
#		parser.c \

OBJS := $(SRCS:.c=.o)
################################################################################

all: libft ${NAME} welcome

${NAME}: ${OBJS} ${LIBS}
	cc -o ${NAME} $^ ${LIBFTFLAGS}

libft:
	make -C ${LIBFT}

.c.o: ${SRCS}
	cc ${CFLAGS} -Ilibft -c -o $@ $<

clean:
	rm -f ${OBJS} && cd ${LIBFT} && make clean && cd ..

fclean: clean
	rm -f ${NAME} ${LIBS}

re: clean all

welcome:
	cat welcome.txt

.PHONY: all clean fclean re
