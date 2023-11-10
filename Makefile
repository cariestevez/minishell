################################################################################
NAME := minishell
CFLAGS := -g -Wall -Wextra -Werror
LIBFTFLAGS := -Ilibft -Llibft -lreadline
LIBFT := ./libft/
LIBS := $(LIBFT)libft.a
SRCS :=  exec.c exec_children.c exec_utils.c free.c test_exec.c #lexer.c parser.c main.c expander.c  
#builtins/pwd.c builtins/cd.c 
OBJS := $(SRCS:.c=.o)
################################################################################

all: libft ${NAME}

${NAME}: ${OBJS} ${LIBS}
	cc $^ ${LIBFTFLAGS} -o ${NAME}

libft:
	make -C ${LIBFT}

.c.o: ${SRCS}
	cc ${CFLAGS} -I. -Ilibft -c -o $@ $<

clean:
	rm -f ${OBJS} && cd ${LIBFT} && make clean

fclean: clean
	rm -f ${NAME} ${LIBS}

re: clean all

.PHONY: all clean fclean re
