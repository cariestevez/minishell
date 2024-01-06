################################################################################
NAME := minishell
CFLAGS := -g -Wall -Wextra -Werror 
LIBFTFLAGS := -Ilibft -Llibft -lreadline -Ireadline/include
INC := -Iinc -I$(LIBFT) -Ireadline/include
LIBFT := ./libft/
LIBS := $(LIBFT)libft.a
BUILTINS := ./src/builtins/
SRCS := ./src/main.c \
		./src/utils_random.c \
		./src/lexer.c \
		./src/lexer_utils.c \
		./src/lexer_utils2.c \
		./src/parser.c \
		./src/parser_utils.c \
		./src/executor.c \
		./src/exec_utils.c \
		./src/free.c \
		./src/subprocess.c \
		./src/expander.c \
		./src/expander_utils.c \
		./src/redirections.c \
		./src/signals_interactive.c \
		./src/signals_non_interactive.c \
		$(BUILTINS)ft_exit.c \
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
