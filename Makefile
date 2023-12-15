################################################################################
NAME := minishell
CFLAGS := -g -Wall -Wextra -Werror 
LIBFTFLAGS := -Ilibft -Llibft -lreadline -Ireadline/include
INC := -Iinc -I$(LIBFT) -Ireadline/include
LIBFT := ./libft/
LIBS := $(LIBFT)libft.a
BUILTINS := ./src/builtins/
SRCS := $(BUILTINS)ft_exit.c \
		./src/main.c \
		./src/lexer.c \
		./src/utils_lexer.c \
		./src/parser.c \
		./src/utils_parser.c \
		./src/executor.c \
		./src/utils_exec.c \
		./src/free.c \
		./src/subprocess.c \
		./src/expander.c \
		./src/utils_expander.c \
		./src/redirections.c \
		$(BUILTINS)ft_cd.c \
		$(BUILTINS)ft_pwd.c \
		$(BUILTINS)ft_echo.c \
		$(BUILTINS)ft_env.c \
		$(BUILTINS)ft_unset.c \
		$(BUILTINS)ft_export.c \

OBJS := $(SRCS:.c=.o)
##################################################################################

all: libft ${NAME} welcome

${NAME}: ${OBJS}
	cc -o ${NAME} $^ ${LIBFTFLAGS} ${LIBS}

libft:
	make -C ${LIBFT}

.c.o:
	cc ${CFLAGS} ${INC} -c -o $@ $<

clean:
	rm -f ${OBJS}
	cd ${LIBFT} && make clean

fclean: clean
	rm -f ${NAME}
	cd ${LIBFT} && make fclean

re: fclean all

welcome:
	cat welcome.txt

.PHONY: all clean fclean re libft
