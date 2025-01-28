CC      = cc
CFLAGS  = -g -Wall -Wextra -Werror -I libft/include -I ./include

NAME = minishell

SRCS =  srcs/builtin/builtin_cd.c \
		srcs/builtin/builtin_echo.c \
		srcs/builtin/builtin_env.c \
		srcs/builtin/builtin_exit.c \
		srcs/builtin/builtin_export.c \
		srcs/builtin/builtin_pwd.c \
		srcs/builtin/builtin_unset.c \
		srcs/builtin/export_process.c \
		srcs/builtin/export_utils.c \
		srcs/execute_cmds/setup_redirections/handle_heredoc_redirection.c \
		srcs/execute_cmds/setup_redirections/setup_redirections.c \
		srcs/execute_cmds/child_process.c \
		srcs/execute_cmds/execute_cmds.c \
		srcs/process_input/lexer/create_tokens_utils.c \
		srcs/process_input/lexer/create_tokens.c \
		srcs/process_input/lexer/create_word_token.c \
		srcs/process_input/lexer/expand_variable_utils.c \
		srcs/process_input/lexer/expand_variable.c \
		srcs/process_input/lexer/lexer.c \
		srcs/process_input/parser/parse_redirection.c \
		srcs/process_input/parser/parser.c \
		srcs/process_input/process_input.c \
		srcs/signals/signals.c \
		srcs/utils/errors.c \
		srcs/utils/free_memory.c \
		srcs/validate_cmds/is_builtin.c \
		srcs/validate_cmds/is_external.c \
		srcs/validate_cmds/validate_cmds.c \
		srcs/main.c 

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
