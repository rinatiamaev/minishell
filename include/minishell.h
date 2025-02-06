/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nlouis <nlouis@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/31 10:27:36 by nlouis            #+#    #+#             */
/*   Updated: 2025/02/06 10:01:08 by nlouis           ###   ########.fr       */
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

# define FILE_PERMISSIONS (S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)

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

typedef struct s_tk
{
	t_tk_type	type;
	char		*value;
	bool		delimiter_quoted;
}	t_tk;

typedef struct s_cmd
{
	char			*name;
	char			**args;
	char			*input_redirect;
	char			*output_redirect;
	char			*heredoc_delimiter;
	int				append;
	int				builtin;
	char			*path;
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

// Create token in lexer
void	create_tk(t_ms *ms, int tk_index, t_tk_type type, char *value);
int		ft_is_operator(char *input, int i);
void	detect_quoted_delimiter(t_tk *tk);
int		collapse_sq_seg(t_ms *ms, int *i, int tk_index);
int		collapse_dq_seg(t_ms *ms, int *i, int tk_index);
void	collapse_uq_seg(t_ms *ms, int *i, int tk_index);
void	create_pipe_tk(t_ms *ms, int *i, int tk_index);
void	create_heredoc_tk(t_ms *ms, int *i, int tk_index);
void	create_redirect_input_tk(t_ms *ms, int *i, int tk_index);
void	create_redirect_output_tk(t_ms *ms, int *i, int tk_index);
void	create_append_output_tk(t_ms *ms, int *i, int tk_index);

// Expand variable in lexer
char	*expand_env_var(t_ms *ms, char *content);

// Utilities for expand variable in lexer
char	*expand_exit_status(t_ms *ms, int exit_status);
char	*expand_env_variable(t_ms *ms, char *content);
char	*process_literal(t_ms *ms, char *content, int *i);
char	*process_variable(t_ms *ms, char *content, int *i);

// PARSER
t_cmd	*parse_tks(t_ms *ms, t_tk **tks);
int		parse_redirections(t_ms *ms, t_cmd *cmd, t_tk **tks, int *i);
void	parse_word(t_ms *ms, t_cmd *cmd, t_tk *tk);

// SIGNALS
void	set_signals_noninteractive(t_ms *ms);
void	set_signals_interactive(t_ms *ms);

// EXECUTE COMMANDS
void	execute_cmd(t_ms *ms, t_cmd *cmd);
void	execute_builtin_cmd(t_ms *ms, t_cmd *cmd);
void	child_process(t_ms *ms, int prev_fd, int next_fd, t_cmd *cmd);
int		setup_redirections(t_ms *ms, t_cmd *cmd);
int	handle_heredoc(t_ms *ms, t_cmd *cmd, t_tk **tks, bool is_piped);

bool	validate_cmds_chunks(t_ms *ms, t_cmd *cmd, char **envp);
bool	is_external(t_ms *ms, t_cmd *cmd, char *cmd_name);
bool	is_builtin(t_cmd *cmd);

// FREE MEMORY
void	free_cmd(t_cmd *cmd);
void	free_tks(t_tk **tks);
void	free_all(t_ms *ms);

// ERRORS
void	syn_err(t_ms *ms, char *error_message);
void	cmd_err(t_cmd *cmd, char *error_message);
void	builtin_err(t_ms *ms, char *error_message);
void	error(t_ms *ms, char *error_message);

// UTILS
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

// Prototypes for export utility functions
bool	is_valid_identifier(char *name);
void	export_err(t_ms *ms, char *arg, char *error_message);
void	add_or_update_env(t_ms *ms, char *var);

void	print_cmd(t_cmd *cmd, int indent);
void	print_tks(t_tk **tks);

#endif
