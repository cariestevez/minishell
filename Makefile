################################################################################
NAME := minishell
CFLAGS := -g -Wall -Wextra -Werror 
LIBFTFLAGS := -Ilibft -Llibft -lreadline  -Ireadline/include
INC := -Iinc -I$(LIBFT) -Ireadline/include
LIBFT := ./libft/
LIBS := $(LIBFT)libft.a
BUILTINS := ./src/builtins/

SRCS := $(BUILTINS)ft_cd.c \
		$(BUILTINS)ft_pwd.c \
		$(BUILTINS)ft_echo.c \
		$(BUILTINS)ft_env.c \
		$(BUILTINS)ft_unset.c \
		$(BUILTINS)ft_export.c \
		$(BUILTINS)ft_cd.c \
		./src/free.c \
		./src/executor.c \
		./src/subprocess.c \
		./src/redirections.c \
		./src/exec_utils.c \
		./src/main.c \
		./src/expander.c \
		./src/lexer.c \
		./src/utils_lexer.c \
		./src/parser.c \
		./src/utils_parser.c \

OBJS := $(SRCS:.c=.o)
################################################################################

all: libft ${NAME} welcome

${NAME}: ${OBJS} ${LIBS}
	cc -o ${NAME} $^ ${LIBFTFLAGS}

libft:
	make -C ${LIBFT}

.c.o: ${SRCS}
	cc ${CFLAGS} ${INC} -c -o $@ $<

clean:
	rm -f ${OBJS} && cd ${LIBFT} && make clean && cd ..

fclean: clean
	rm -f ${NAME} ${LIBS}

re: clean all

welcome:
	cat welcome.txt

.PHONY: all clean fclean re
