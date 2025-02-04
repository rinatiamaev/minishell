/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nlouis <nlouis@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/31 10:27:36 by nlouis            #+#    #+#             */
/*   Updated: 2025/02/04 15:04:03 by nlouis           ###   ########.fr       */
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

# define MAX_TKS 1024

# define FILE_PERMISSIONS (S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)

typedef enum e_tk_type
{
	TK_WORD,
	TK_PIPE,
	TK_REDIRECT_INPUT,
	TK_REDIRECT_OUTPUT,
	TK_APPEND_OUTPUT,
	TK_HEREDOC
}	t_tk_type;

typedef struct s_tk
{
	t_tk_type	type;
	char		*value;
	bool		is_delimiter;
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
	const char	*input;
	t_tk		**tks;
	t_cmd		*cmd;
}	t_ms;

typedef struct s_collapse
{
	t_ms		*ms;
	const char	*input;
	int			*i;
	char		**buffer;
	bool		is_delimiter;
}	t_collapse;

// PROCESS INPUT
int		process_input(t_ms *ms);

// LEXER
t_tk	**lexer(t_ms *ms, const char *input);

// Create token in lexer
t_tk	**initialize_tks(t_ms *ms);
t_tk	*create_tk(t_ms *ms, t_tk_type type, char *value);
int		ft_is_operator(const char *input, int i);
void	detect_quoted_delimiter(t_tk *tk);
char	*ft_strjoin_free(char *s1, char *s2);
void	collapse_sq_seg(t_collapse	*col);
void	collapse_dq_seg(t_collapse	*col);
void	collapse_uq_seg(t_collapse	*col);
t_tk	*create_pipe_tk(t_ms *ms, int *i);
t_tk	*create_heredoc_tk(t_ms *ms, int *i);
t_tk	*create_redirect_input_tk(t_ms *ms, int *i);
t_tk	*create_redirect_output_tk(t_ms *ms, int *i);
t_tk	*create_append_output_tk(t_ms *ms, int *i);

// Expand variable in lexer
char	*expand_env_var(t_ms *ms, const char *content);

// Utilities for expand variable in lexer
char	*expand_exit_status(t_ms *ms, int exit_status);
char	*expand_env_variable(t_ms *ms, const char *content);
char	*process_literal(t_ms *ms, const char *content, int *i);
char	*process_variable(t_ms *ms, const char *content, int *i);

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
int		handle_heredoc(t_ms *ms, t_cmd *cmd, t_tk **tks);

bool	validate_cmds(t_ms *ms, t_cmd *cmd, char **envp);
bool	is_external(t_ms *ms, t_cmd *cmd, char *cmd_name);
bool	is_builtin(t_cmd *cmd);

// FREE MEMORY
void	free_cmd(t_cmd *cmd);
void	free_tks(t_tk **tks);
void	free_all(t_ms *ms);

// ERRORS
void	syn_err(t_ms *ms, char *error_message);
void	cmd_err(t_ms *ms, char *error_message);
void	builtin_err(t_ms *ms, char *error_message);
void	error(t_ms *ms, char *error_message);

// BUILTINS
void	builtin_echo(t_ms *ms, t_cmd *cmd);
void	builtin_cd(t_ms *ms, t_cmd *cmd);
void	builtin_pwd(t_ms *ms);
void	builtin_unset(t_ms *ms, t_cmd *cmd);
void	builtin_env(t_ms *ms);
void	builtin_exit(t_ms *ms, t_cmd *cmd);
void	builtin_export(t_ms *ms, t_cmd *cmd);

// Prototypes for export utility functions
bool	is_valid_identifier(const char *name);
void	export_err(t_ms *ms, const char *arg, char *error_message);
void	add_or_update_env(t_ms *ms, const char *var);

void	print_cmd(t_cmd *cmd, int indent);
void	print_tks(t_tk **tks);

#endif
