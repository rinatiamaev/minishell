CC      = cc
CFLAGS  = -g -Wall -Wextra -Werror -I libft/include -I ./include

NAME = minishell

SRCS =  srcs/free_memory.c \
		srcs/lexical_analyser.c \
		srcs/main.c \
		srcs/utils.c \
		srcs/parser.c \
		srcs/validate_cmds.c \
		srcs/execute_cmd.c \
		srcs/builtin_cmds.c \
		srcs/setup_redirections.c \
		srcs/signals.c \
		srcs/debugging_print.c \
		srcs/builtin/builtin_echo.c \
		srcs/builtin/builtin_cd.c \
		srcs/builtin/builtin_pwd.c \
		srcs/builtin/builtin_export.c \
		srcs/builtin/builtin_unset.c \
		srcs/builtin/builtin_env.c \
		srcs/builtin/builtin_exit.c \
		srcs/builtin/export_process.c \
		srcs/builtin/export_utils.c

OBJS_MAIN   = $(SRCS:.c=.o)

LIBFT   = libft/libft.a

all: $(NAME)

$(NAME): $(OBJS_MAIN) $(LIBFT)
	$(CC) $(CFLAGS) -o $(NAME) $(OBJS_MAIN) $(LIBFT) -lreadline

$(LIBFT):
	make -C libft

%.o: %.c include/minishell.h
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS_MAIN)
	make -C libft clean

fclean: clean
	rm -f $(NAME)
	make -C libft fclean

re: fclean all

.PHONY: all clean fclean re
