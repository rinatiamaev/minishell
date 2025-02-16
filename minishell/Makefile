CC      = cc
CFLAGS  = -Wall -Wextra -Werror -I libft/include -I ./include # -g -fsanitize=address -fsanitize=undefined

NAME = minishell

SRCS =  srcs/builtin/builtin_cd.c \
		srcs/builtin/builtin_echo.c \
		srcs/builtin/builtin_env.c \
		srcs/builtin/builtin_exit.c \
		srcs/builtin/builtin_pwd.c \
		srcs/builtin/builtin_unset.c \
		srcs/builtin/export/builtin_export.c \
		srcs/builtin/export/export_utils.c \
		srcs/execute_cmds/setup_redir/setup_heredoc.c \
		srcs/execute_cmds/setup_redir/setup_redir.c \
		srcs/execute_cmds/setup_redir/setup_input_redir.c \
		srcs/execute_cmds/setup_redir/setup_ouput_redir.c \
		srcs/execute_cmds/child_process.c \
		srcs/execute_cmds/execute_builtin_cmd.c \
		srcs/execute_cmds/execute_cmds.c \
		srcs/process_input/expand_tks/expand_tks.c \
		srcs/process_input/lexer/collapse_input.c \
		srcs/process_input/lexer/create_delim_tk.c \
		srcs/process_input/lexer/create_op_tk.c \
		srcs/process_input/lexer/create_word_tk.c \
		srcs/process_input/lexer/lexer_utils.c \
		srcs/process_input/lexer/lexer.c \
		srcs/process_input/parse_tks/parse_pipe.c \
		srcs/process_input/parse_tks/parse_output_redir.c \
		srcs/process_input/parse_tks/parse_input_redir.c \
		srcs/process_input/parse_tks/parse_redir.c \
		srcs/process_input/parse_tks/parse_tks.c \
		srcs/process_input/parse_tks/parse_tks_utils.c \
		srcs/process_input/parse_tks/parse_word.c \
		srcs/process_input/process_input.c \
		srcs/signals/signals.c \
		srcs/utils/errors.c \
		srcs/utils/free_memory.c \
		srcs/utils/x_memory_utils.c \
		srcs/utils/expand_var_utils.c \
		srcs/utils/expand_var.c \
		srcs/process_input/assign_cmd_type/is_builtin.c \
		srcs/process_input/assign_cmd_type/is_external.c \
		srcs/process_input/assign_cmd_type/assign_cmd_type.c \
		srcs/main.c

OBJSDIR = objs
OBJS   = $(addprefix $(OBJSDIR)/,$(SRCS:.c=.o))

LIBFT   = libft/libft.a

all: $(NAME)

$(NAME): $(OBJS) $(LIBFT)
	$(CC) $(CFLAGS) -o $(NAME) $(OBJS) $(LIBFT) -lreadline

$(LIBFT):
	make -C libft

$(OBJSDIR)/%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS)
	make -C libft clean

fclean: clean
	rm -f $(NAME)
	make -C libft fclean

re: fclean all

.PHONY: all clean fclean re
