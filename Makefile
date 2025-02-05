CC      = cc
CFLAGS  = -Wall -Wextra -Werror -I libft/include -I ./include -g -fsanitize=address -fsanitize=undefined

NAME = minishell

SRCS =  srcs/builtin/builtin_cd.c \
		srcs/builtin/builtin_echo.c \
		srcs/builtin/builtin_env.c \
		srcs/builtin/builtin_exit.c \
		srcs/builtin/builtin_pwd.c \
		srcs/builtin/builtin_unset.c \
		srcs/builtin/export/builtin_export.c \
		srcs/builtin/export/export_utils.c \
		srcs/execute_cmds/setup_redirections/setup_heredoc.c \
		srcs/execute_cmds/setup_redirections/setup_redirections.c \
		srcs/execute_cmds/child_process.c \
		srcs/execute_cmds/execute_cmds.c \
		srcs/process_input/lexer/collapse_adjacent_str.c \
		srcs/process_input/lexer/create_token.c \
		srcs/process_input/lexer/create_tokens.c \
		srcs/process_input/lexer/expand_variable_utils.c \
		srcs/process_input/lexer/expand_variable.c \
		srcs/process_input/lexer/lexer_utils.c \
		srcs/process_input/lexer/lexer.c \
		srcs/process_input/parser/parse_redirection.c \
		srcs/process_input/parser/parse_word.c \
		srcs/process_input/parser/parser.c \
		srcs/process_input/process_input.c \
		srcs/signals/signals.c \
		srcs/utils/errors.c \
		srcs/utils/free_memory.c \
		srcs/utils/x_memory_utils.c \
		srcs/validate_cmds/is_builtin.c \
		srcs/validate_cmds/is_external.c \
		srcs/validate_cmds/validate_cmds.c \
		srcs/main.c

OBJSDIR = objs
OBJS   = $(addprefix $(OBJSDIR)/,$(SRCS:.c=.o))

LIBFT   = libft/libft.a

all: $(NAME)

$(NAME): $(OBJS) $(LIBFT)
	$(CC) $(CFLAGS) -o $(NAME) $(OBJS) $(LIBFT) -lreadline

$(LIBFT):
	make -C libft

$(OBJSDIR)/%.o: %.c include/minishell.h
	mkdir -p $(@D)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS)
	make -C libft clean

fclean: clean
	rm -f $(NAME)
	make -C libft fclean

re: fclean all

.PHONY: all clean fclean re
