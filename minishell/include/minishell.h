/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nlouis <nlouis@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/31 10:27:36 by nlouis            #+#    #+#             */
/*   Updated: 2025/02/14 13:24:55 by nlouis           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "libft.h"
# include <stdio.h>
# include <stdlib.h>
# include <stdbool.h>
# include <ctype.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <sys/stat.h>
# include <sys/ioctl.h>
# include <fcntl.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <errno.h>
# include <signal.h>
# include <unistd.h>
# include <string.h>

# define BOLD_BLUE "\033[1;34m"
# define RED "\033[0;31m"
# define RESET "\033[0m"

# define MAX_TKS 24000

typedef enum e_tk_type
{
	TK_WORD,
	TK_PIPE,
	TK_REDIRECT_INPUT,
	TK_REDIRECT_OUTPUT,
	TK_APPEND_OUTPUT,
	TK_HEREDOC,
	TK_HEREDOC_DELIMITER
}	t_tk_type;

// qs -> quote status
typedef enum e_qs
{
	QUOTE_NONE,
	QUOTE_SINGLE,
	QUOTE_DOUBLE
}	t_qs;

typedef struct s_tk
{
	t_tk_type		type;
	char			*value;
	bool			delimiter_quoted;
	t_qs			qs;
}	t_tk;

typedef struct s_redir
{
	bool	is_input;
	bool	is_heredoc;
	bool	is_append;
	char	*filename;
	char	*delimiter;
	bool	delimiter_quoted;
	int		heredoc_fd;
}	t_redir;

typedef struct s_cmd
{
	char			*name;
	char			**args;
	t_redir			**input_redirs;
	t_redir			**output_redirs;
	int				builtin;
	char			*path;
	bool			has_command;
	struct s_cmd	*pipe_to;
}	t_cmd;

typedef struct s_ms
{
	char		**envp;
	int			exit_status;
	char		*input;
	t_tk		**tks;
	t_cmd		*cmd;
}	t_ms;

// PROCESS INPUT
int		process_input(t_ms *ms);

// LEXER
int		lexer(t_ms *ms);

// LEXER UTILS
t_tk	*new_tk(t_ms *ms, t_tk_type type, const char *value, t_qs qs);
void	create_pipe_tk(t_ms *ms, int *i, int *tk_index);
void	create_redir_input_tk(t_ms *ms, int *i, int *tk_index);
void	create_redir_output_tk(t_ms *ms, int *i, int *tk_index);
void	create_append_output_tk(t_ms *ms, int *i, int *tk_index);
void	create_heredoc_tk(t_ms *ms, int *i, int *tk_index);
t_tk	*create_delimiter_tk(t_ms *ms, int *i);
int		collapse_sq_seg(t_ms *ms, int *i, int tk_index);
int		collapse_dq_seg(t_ms *ms, int *i, int tk_index);
void	collapse_uq_seg(t_ms *ms, int *i, int tk_index);
t_tk	*create_word_tk(t_ms *ms, int *i, int tk_index);
int		ft_is_operator(char *input, int i);
int		ft_isspace_operator(char *input, int i);

// EXPANDER
int		expand_tks(t_ms *ms);

// PARSER
t_cmd	*parse_tks(t_ms *ms, t_tk **tks);
t_cmd	*new_cmd(t_ms *ms, t_cmd *head);
t_cmd	*parse_pipe(t_ms *ms, t_tk **tks, int *i);
int		parse_word(t_ms *ms, t_cmd *cmd, t_tk *tk);
int		append_input_redir(t_ms *ms, t_cmd *cmd, t_tk **tks, int *i);
int		append_output_redir(t_ms *ms, t_cmd *cmd, t_tk **tks, int *i);
int		parse_redir(t_ms *ms, t_cmd *cmd, t_tk **tks, int *i);

// ASSIGN CMD TYPE
void	assign_cmd_type(t_ms *ms, t_cmd *cmd);
bool	is_external(t_ms *ms, t_cmd *cmd, char *cmd_name);
bool	is_builtin(t_cmd *cmd);

// SIGNALS
void	set_signals_noninteractive(t_ms *ms);
void	set_signals_interactive(t_ms *ms);

// EXECUTE COMMANDS
void	execute_cmd(t_ms *ms, t_cmd *cmd);
int		read_all_heredocs(t_ms *ms, t_cmd *cmd);
void	execute_builtin_cmd(t_ms *ms, t_cmd *cmd);
void	child_process(t_ms *ms, int prev_fd, int next_fd, t_cmd *cmd);
int		handle_one_output_redir(t_ms *ms, t_redir *rd);
int		handle_one_input_redir(t_ms *ms, t_redir *rd);
int		setup_redir(t_ms *ms, t_cmd *cmd);

// FREE MEMORY
void	free_cmd(t_cmd *cmd);
void	free_tks(t_tk **tks);
void	free_all(t_ms *ms);

// ERRORS
void	syn_err(t_ms *ms, char *error_message);
void	cmd_err(t_cmd *cmd, char *error_message);
void	builtin_err(t_ms *ms, char *error_message);
void	error(t_ms *ms, char *error_message);
void	redir_err(t_redir *rd, char *error_message);

// UTILS
void	*x_malloc(t_ms *ms, size_t size);
char	*x_strdup(t_ms *ms, const char *s);
char	*x_substr(t_ms *ms, const char *s, unsigned int start, int len);
char	*x_strjoin_free(t_ms *ms, char *s1, char *s2);

// BUILTINS
void	builtin_echo(t_ms *ms, t_cmd *cmd);
void	builtin_cd(t_ms *ms, t_cmd *cmd);
void	builtin_pwd(t_ms *ms);
void	builtin_unset(t_ms *ms, t_cmd *cmd);
void	builtin_env(t_ms *ms);
void	builtin_exit(t_ms *ms, t_cmd *cmd);
void	builtin_export(t_ms *ms, t_cmd *cmd);

// Prototypes for expand_variable.c
char	*expand_env_var(t_ms *ms, char *content);
char	*process_variable(t_ms *ms, char *content, int *i);
char	*process_literal(t_ms *ms, char *content, int *i);

// Prototypes for export utility functions
bool	is_valid_identifier(char *name);
void	export_err(t_ms *ms, char *arg, char *error_message);
void	add_or_update_env(t_ms *ms, char *var);

#endif
