################################################################################
NAME := minishell
CFLAGS := -g -Wall -Wextra -Werror -I.
LIBFTFLAGS := -Ilibft -Llibft -lreadline
LIBFT := ./libft/
LIBS := $(LIBFT)libft.a
SRCS :=  exec.c exec_children.c exec_redir.c exec_utils.c test_exec.c free.c  lexer.c expander.c #parser.c
OBJS := $(SRCS:.c=.o)
################################################################################

all: libft ${NAME}

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

.PHONY: all clean fclean re
