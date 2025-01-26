/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nlouis <nlouis@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/02 18:32:29 by nlouis            #+#    #+#             */
/*   Updated: 2025/01/22 10:08:52 by nlouis           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* parse_pipe()
 * 	- advances the tk index (*i) past the `|` tk.
 *	- validates the presence of the next tk:
 *		- if no tk exists or the next tk is not a `tk_WORD`,
 *		reports a syntax error
 *	- calls `parse_tks` recursively to parse the tks after the pipe (`|`)
 *	creating the next cmd in the pipeline
 *	- assigns the newly parsed cmd to the `pipe_to` field of the current
 *	cmd structure
 *	- returns the updated cmd structure, or 
 *	`NULL` if a syntax error occurs or if parsing the piped cmd fails
 */
static t_cmd	*parse_pipe(t_ms *ms, t_cmd *cmd, t_tk **tks, int *i)
{
	(*i)++;
	if (!tks[*i] || tks[*i]->type != TK_WORD)
	{
		syn_err(ms, "invalid tk near '|'");
		return (NULL);
	}
	cmd->pipe_to = parse_tks(ms, &tks[*i]);
	if (!cmd->pipe_to)
		return (NULL);
	return (cmd);
}

/* parse_heredoc()
 *	- advances the tk index (*i) past the `<<` tk.
 *	- validates the presence of the next tk:
 *		- if no tk exists, reports a syntax error
 *	- validates the type of the next tk:
 *		- if the tk is not a `tk_WORD`, reports a syntax error
 *	- if a valid delimiter tk is found:
 *		- copies the value of the tk (the heredoc delimiter) into
 *		`cmd->heredoc_delimiter`
 *	- returns: `0` on success, `-1` if a syntax error occurs
 */
static int	parse_heredoc(t_ms *ms, t_cmd *cmd, t_tk **tks, int *i)
{
	(*i)++;
	if (!tks[*i])
	{
		syn_err(ms, "expected a delimiter after '<<'");
		return (-1);
	}
	if (tks[*i]->type != TK_WORD)
	{
		syn_err(ms, "invalid tk after '<<'");
		return (-1);
	}
	cmd->heredoc_delimiter = ft_strdup(tks[*i]->value);
	if (!cmd->heredoc_delimiter)
		error(ms, "Error ft_strdup() failed in parse_heredoc");
	return (0);
}

/* parse_redirect_output()
 *	- advances the tk index (*i) past the redirection tk (`>` or `>>`).
 *	- checks for the next tk:
 *		- if no tk exists, reports a syntax error
 *		- if the next tk is not a `tk_WORD`, reports a syntax error
 *	- if a valid tk is found:
 *		- copies the value of the tk (the file name) into
 *		`cmd->output_redirect` using `ft_strdup`
 *		- sets the `cmd->append` flag to the provided `append` value
 *	- returns: `0` on success, `-1` if a syntax error occurs.
 */
static int	parse_redirect_output(t_ms *ms, t_cmd *cmd,
									t_tk **tks, int *i, int append)
{
	(*i)++;
	if (!tks[*i])
	{
		syn_err(ms, "expected a file after '>' or '>>'");
		return (-1);
	}
	if (tks[*i]->type != TK_WORD)
	{
		syn_err(ms, "invalid tk after '>' or '>>'");
		return (-1);
	}
	cmd->output_redirect = ft_strdup(tks[*i]->value);
	if (!cmd->output_redirect)
		error(ms, "Error ft_strdup() failed in parse_redirect_output");
	cmd->append = append;
	return (0);
}

/* parse_redirect_input()
 *	- advances the tk index (*i) past the `<` tk.
 *	- checks for the next tk:
 *		- if no tk exists, reports a syntax error
 *		- if the next tk is not a `tk_WORD`, reports a syntax error
 *	-if a valid tk is found:
 *		- copies the value of the tk (the file name) into
 *		`cmd->input_redirect` using `ft_strdup`
 *	- returns: `0` on success, `-1` if a syntax error occurs
 */
static int	parse_redirect_input(t_ms *ms, t_cmd *cmd, t_tk **tks, int *i)
{
	(*i)++;
	if (!tks[*i])
	{
		syn_err(ms, "expected a file after '<'");
		return (-1);
	}
	if (tks[*i]->type != TK_WORD)
	{
		syn_err(ms, "invalid tk after '<'");
		return (-1);
	}
	cmd->input_redirect = ft_strdup(tks[*i]->value);
	if (!cmd->input_redirect)
		error(ms, "Error ft_strdup() failed in parse_redirect_input");
	return (0);
}

/* parse_word()
 *	-checks if `cmd->name` is already set:
 *		- if not, assigns the word tk value to `cmd->name`
 *		- allocates memory for the string using `ft_strdup`
 *	- if `cmd->name` is already set:
 *		-treats the word tk as an argument
 *		- calculates the current length of `cmd->args` (the argument array)
 *		- reallocates memory for the argument array to accommodate the new
 *		argument using `ft_realloc`
 *		- appends the new argument to the array.
 *		- ensures the argument array is null-terminated.
 */
static void	parse_word(t_ms *ms, t_cmd *cmd, t_tk *tk)
{
	int		args_len;
	char	**new_args;

	if (!cmd->name)
	{
		cmd->name = ft_strdup(tk->value);
		if (!cmd->name)
			error(ms, "Error ft_strdup() failed in parse_word_tk");
	}
	else
	{
		args_len = 0;
		while (cmd->args && cmd->args[args_len])
			args_len++;
		new_args = ft_realloc(cmd->args, args_len * sizeof(char *),
								sizeof(char *) * (args_len + 2));
		if (!new_args)
			error(ms, "Error ft_realloc() failed in parse_word_tk");
		cmd->args = new_args;
		cmd->args[args_len] = ft_strdup(tk->value);
		if (!cmd->args[args_len])
			error(ms, "Error ft_strdup() failed in parse_word_tk");
		cmd->args[args_len + 1] = NULL;
	}
}

/**
 * parse_redirections()
 *	- checks the type of the current tk:
 *		- `tk_REDIRECT_INPUT` (`<`): Parses input redirection and updates
 *		`cmd->input_redirect`.
 *		- `tk_REDIRECT_OUTPUT` (`>`): Parses output redirection and updates
 *		`cmd->output_redirect`.
 *		- `tk_APPEND_OUTPUT` (`>>`): Parses append output redirection and
 *		updates `cmd->output_redirect` with `append` mode enabled.
 *		- `tk_HEREDOC` (`<<`): Parses heredoc redirection and updates
 *		`cmd->heredoc_delimiter`.
 *	- calls the appropriate parsing function based on the tk type:
 *    - `parse_redirect_input()`, `parse_redirect_output()`, `parse_heredoc()`
 *	- advances the tk index (`*i`) within the called parsing functions
 *	- returns: `0` on success, `-1` if asyntax error occurs during parsing
 *	(missing or invalid tk after a redirection operator)
 */
static int parse_redirections(t_ms *ms, t_cmd *cmd, t_tk **tks, int *i)
{
	if (tks[*i]->type == TK_REDIRECT_INPUT)
		return (parse_redirect_input(ms, cmd, tks, i));
	if (tks[*i]->type == TK_REDIRECT_OUTPUT)
		return (parse_redirect_output(ms, cmd, tks, i, 0));
	if (tks[*i]->type == TK_APPEND_OUTPUT)
		return (parse_redirect_output(ms, cmd, tks, i, 1));
	if (tks[*i]->type == TK_HEREDOC)
		return (parse_heredoc(ms, cmd, tks, i));
	return (0);
}

/* initialize_cmd()
 *	- allocates memory for a `t_cmd` structure
 *	- uses `ft_memset()` to zero-initialize the structure
 *	- returns: allocated and initialized `t_cmd` structure.
 */
static t_cmd	*initialize_cmd(t_ms *ms)
{
	t_cmd	*cmd;

	cmd = malloc(sizeof(t_cmd));
	if (!cmd)
		error(ms, "Error malloc() failed in initialize_cmd");
	ft_memset(cmd, 0, sizeof(t_cmd));
	return (cmd);
}

/* parse_tks()
 *	- initializes a new `t_cmd` structure to represent the parsed cmd
 *	- iterates through the tks array to process each tk:
 *		- `tk_WORD`: Calls `parse_word()` to set the cmd or add argument
 *		- `tk_PIPE`: Indicates a pipeline; calls `parse_pipe()` to link the 
 *		current cmd to the next in the pipeline and exits the loop
 *		- redirection tks (`<`, `>`, `>>`, `<<`): Calls parse_redirections()
 *      to handle input/output redirections or heredoc delimiters
 *	- returns: ptr to the root `t_cmd` structure representing the parsed
 *	cmd, NULL in case of syntax errors or memory allocation failures.
 */
t_cmd *parse_tks(t_ms *ms, t_tk **tks)
{
	t_cmd	*cmd;
	int			i;

	i = 0;
	cmd = initialize_cmd(ms);
	while (tks[i] != NULL)
	{
		if (tks[i]->type == TK_WORD)
			parse_word(ms, cmd, tks[i]);
		else if (tks[i]->type == TK_PIPE)
		{
			if (parse_pipe(ms, cmd, tks, &i) == NULL)
				return (NULL);
			break ;
		}
		else
		{
			if (parse_redirections(ms, cmd, tks, &i) == -1)
				return (NULL);
		}
		i++;
	}
	return (cmd);
}
