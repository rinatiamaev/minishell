/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexical_analyser.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nlouis <nlouis@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/02 16:20:10 by nlouis            #+#    #+#             */
/*   Updated: 2025/01/17 11:18:26 by nlouis           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	skip_whitespace(const char *input, int *i)
{
	while (ft_isspace(input[*i]))
		(*i)++;
}

/* create_tk()
 *	- allocates and initializes a tk with the given type and value.
 *	- duplicates the value string if provided; otherwise, sets it to NULL.
 *	- handles memory allocation errors via the error handler.
 *	- returns: a pointer to the created tk.
 */
static t_tk	*create_tk(t_ms *ms, t_tk_type type, char *value)
{
	t_tk	*tk;

	tk = malloc(sizeof(t_tk));
	if (!tk)
		error(ms, "malloc() failed in create_tk()");
	tk->type = type;
	if (value)
	{
		tk->value = ft_strdup(value);
		if (!tk->value)
			error(ms, "ft_strdup() failed in create_tk()");
	}
	else
		tk->value = NULL;
	return (tk);
}

/* get_env_variable()
 *	- retrieves the value of an environment variable from
 *	the shell's environment
 *	- calculates the length of the variable name using `ft_strlen`
 *	- iterates through the `ms->envp` array of environment strings
 *		- each string in `envp` is formatted as `VAR=value`
 *	- compares the variable name with the beginning of each environment string,
 *	ensures the match ends with an '=' character to avoid partial matches
 *	- if a match is found, duplicates the value portion (`value`) of the
 *	environment string
 *	- a string containing the value of the environment variable, or `NULL`
 *	if the variable is not found
 */

static char	*get_env_variable(t_ms *ms, const char *var_name)
{
	size_t	name_len;
	char	**env;

	name_len = ft_strlen(var_name);
	env = ms->envp;
	while (*env)
	{
		if (ft_strncmp(*env, var_name, name_len) == 0
			&& (*env)[name_len] == '=')
			return (ft_strdup(*env + name_len + 1));
		env++;
	}
	return (NULL);
}

/* expand_env_variable()
 *	- expands an environment variable by retrieving its value from the shell's
 *	environment variables
 *	- calls `get_env_variable` to retrieve the value of the environment variable
 *	- if the variable is not found, the value is the content
 *	- returns: a string containing the variable's value or an empty string if
 *	the variable is not found
 */

static char	*expand_env_variable(t_ms *ms, const char *content)
{
	char	*value;

	value = get_env_variable(ms, content);
	if (!value)
		value = ft_strdup("");
	if (!value)
		error(ms, "ft_strdup() failed in expand_env_variable()");
	return (value);
}

/* expand_exit_status()
 *	- converts the given exit status to a string using ft_itoa.
 *	- returns the string or exits on memory error.
 */
static char	*expand_exit_status(t_ms *ms, int exit_status)
{
	char	*status;

	status = ft_itoa(exit_status);
	if (!status)
		error(ms, "ft_itoa() failed in expand_exit_status()");
	return (status);
}

/* process_variable()
 *	- extracts and expands an environment variable or the exit status ('$?')
 *	from the input string
 *	- checks if the variable is '$?' to expand it to the shell's exit status
 *	- otherwise, identifies the variable name by iterating through valid
 *	characters (alphanumeric or underscore)
 *	- extracts the variable name using `ft_substr` and expands it using
 *  `expand_env_variable`
 *	- returns the expanded variable value as a newly allocated string
 */

static char	*process_variable(t_ms *ms, const char *content, int *i)
{
	char	*var_name;
	char	*expanded;
	int		start;

	start = *i;
	(*i)++;
	if (content[*i] == '?')
	{
		(*i)++;
		return (expand_exit_status(ms, ms->exit_status));
	}
	while (content[*i] != '\0'
		&& (ft_isalnum(content[*i]) || content[*i] == '_'))
		(*i)++;
	var_name = ft_substr(content, start + 1, (*i - (start + 1)));
	if (!var_name)
		error(ms, "ft_substr() failed in process_variable()");
	expanded = expand_env_variable(ms, var_name);
	free(var_name);
	return (expanded);
}

/* process_literal()
 *	- accumulates literal characters in 'word' until encountering a '$' or '\0'
 *	- returns the substring
 */
static char	*process_literal(t_ms *ms, const char *content, int *i)
{
	int		start;
	char	*literal;

	start = *i;
	while (content[*i] != '\0' && content[*i] != '$')
		(*i)++;
	literal = ft_substr(content, start, *i - start);
	if (!literal)
		error(ms, "ft_substr() failed in process_literal()");
	return (literal);
}

/* expand_env_in_word()
 *	- handles basic environment variable expansion for unquoted words
 *	- if the entire string is "$?", expands it to the shell's last exit status
 *	- if the string starts with '$' followed by a non-null character, expands
 *   the variable via expand_env_variable()
 *	- otherwise, returns a copy of the original content
 *	- returns the expanded string on success
 */
static char	*expand_env_in_word(t_ms *ms, const char *content)
{
	char	*output;

	if (ft_strcmp(content, "$?") == 0)
		return (expand_exit_status(ms, ms->exit_status));
	else if (content[0] == '$' && content[1] != '\0')
		return (expand_env_variable(ms, &content[1]));
	output = ft_strdup(content);
	if (!output)
		error(ms, "Error ft_substr() failed in expand_env_in_word");
	return (output);
}

/* create_word_tk()
 *	- handles literals and words that are not enclosed by quotes or are
 *	special operators (like `|`, `<`, `>`).
 *	- skips over any unsupported characters (`;` and `\`), reporting a
 *	syntax error if encountered
 	- identifies the start of the word and iterates until encountering:
 *    - whitespace
 *    - Special characters (`|`, `<`, `>`)
 *    - End of input (`\0`)
 *	- extracts the word using `ft_substr`
 *	- expands environment variables in the word using `expand_env_in_word`
 *	- returns a `tk_WORD` with the expanded content, NULL if an unsupported
 *	character is encountered (`;` or `\`)
 */
static t_tk	*create_word_tk(t_ms *ms, const char *input, int *i)
{
	int		start;
	char	*raw_content;
	char	*expanded;
	t_tk	*tk;

	start = *i;
	while (!ft_isspace(input[*i]) && input[*i] != '|'
		&& input[*i] != '<' && input[*i] != '>' && input[*i] != '\0')
	{
		if (input[*i] == ';' || input[*i] == '\\')
		{
			syn_err(ms, "characters ';' and '\\' are not supported");
			return (NULL);
		}
		(*i)++;
	}
	raw_content = ft_substr(input, start, *i - start);
	if (!raw_content)
		error(ms, "Error ft_substr() failed in create_word_tk");
	expanded = expand_env_in_word(ms, raw_content);
	free(raw_content);
	tk = create_tk(ms, TK_WORD, expanded);
	free(expanded);
	return (tk);
}

/* expand_in_double_quote()
 *	- handles environment variable expansion (`$VARIABLE`) and literal
 *	text inside the double quotes
 *	- iterates through the content, processing variables and literals:
 *		- variables (`$VAR`) are processed using `process_variable`
 *		- literal segments are handled with `process_literal`
 *	- combines expanded variables and literals into a single string
 *	- returns string with all variables expanded
 */
static char	*expand_in_double_quote(t_ms *ms, const char *content)
{
	char	*output;
	char	*tmp;
	char	*processed;
	int		i;

	output = ft_strdup("");
	if (!output)
		error(ms, "Error ft_strdup() failed in expand_in_double_quote");
	i = 0;
	while (content[i] != '\0')
	{
		if (content[i] == '$' && content[i + 1] != '\0')
			processed = process_variable(ms, content, &i);
		else
			processed = process_literal(ms, content, &i);
		tmp = ft_strjoin(output, processed);
		if (!tmp)
			error(ms, "Error ft_strjoin() failed in expand_in_double_quote");
		free(processed);
		free(output);
		output = tmp;
	}
	return (output);
}

/* create_double_quote_tk()
 *	- skips the opening double quote by advancing the index (*i)
 *	- identifies the starting point of the content within the double
 *	quotes in start
 *	- handles unclosed double quotes by reporting a syntax error and
 *	returning NULL
 *	- extracts the content between the quotes using `ft_substr`
 *	- expands environment variables within the double-quoted content
 *	- creates a tk of type `tk_WORD` with the expanded content
 *	- handles memory allocation errors for the raw content
 *	- returns the created tk on success or NULL if a syntax error
 *	(unclosed quote) occurs
 */

static t_tk	*create_double_quote_tk(t_ms *ms, const char *input, int *i)
{
	int		start;
	char	*raw_content;
	char	*expanded;
	t_tk	*tk;

	(*i)++;
	start = *i;
	while (input[*i] != '"' && input[*i] != '\0')
		(*i)++;
	if (input[*i] == '\0')
	{
		syn_err(ms, "unclosed double quote");
		return (NULL);
	}
	raw_content = ft_substr(input, start, *i - start);
	if (!raw_content)
		error(ms, "Error ft_substr() failed in create_double_quote_tk");
	(*i)++;
	expanded = expand_in_double_quote(ms, raw_content);
	free(raw_content);
	tk = create_tk(ms, TK_WORD, expanded);
	free(expanded);
	return (tk);
}

/* create_single_quote_tk()
 *	- skips the opening single quote by advancing the index (*i)
 *	- identifies the starting point of the content within the sinlge
 *	quotes in start
 *	- handles unclosed single quotes by reporting a syntax error and
 *	returning NULL
 *	- extracts the content between the quotes using `ft_substr`
 *	- creates a `tk_WORD` tk with the extracted content
 *	- returns the created tk, or NULL on syntax error
 */

static t_tk	*create_single_quote_tk(t_ms *ms, const char *input, int *i)
{
	int		start;
	char	*content;
	t_tk	*tk;

	(*i)++;
	start = *i;
	while (input[*i] != '\'' && input[*i] != '\0')
		(*i)++;
	if (input[*i] == '\0')
	{
		syn_err(ms, "unclosed single quote");
		return (NULL);
	}
	content = ft_substr(input, start, *i - start);
	if (!content)
		error(ms, "Error ft_substr() failed in create_single_quote_tk");
	(*i)++;
	tk = create_tk(ms, TK_WORD, content);
	free(content);
	return (tk);
}

/* create_heredoc_tk
*	- creates a tk for the '<<' (heredoc) operator
*	- ensures that '<<' is not the first element of the cmd line
*	- if valid, advances the input index past the '<<' and creates the tk
*	- returns a tk if valid or NULL if not valid or if '<<' is the first
*	element, as it would be a syntax error
*/
static t_tk	*create_heredoc_tk(t_ms *ms, const char *input, int *i)
{
	(void)input;
	if (*i == 0)
	{
		syn_err(ms, "'<<' can not be the first element of the cmd line");
		return (NULL);
	}
	*i += 2;
	return (create_tk(ms, TK_HEREDOC, "<<"));
}

/**
 * create_redirection_tk()
 *	- identifies and creates tks for the redirection operators:
 *		- `<<` for HEREDOC
 *		- `>>` for append output redirection tk_APPEND_OUTPUT
 *		- `>` for standard output redirection tk_REDIRECT_OUTPUT
 *		- `<` for standard input redirection tk_REDIRECT_INPUT
 *	- advances the position pointer `i` as it processes the operators
 *	- returns the tk or NULL in case of syntax error, should never
 *	the last return statement
 */
static t_tk	*create_redirection_tk(t_ms *ms, const char *input, int *i)
{
	if (input[*i] == '<' && input[*i + 1] == '<')
		return (create_heredoc_tk(ms, input, i));
	else if (input[*i] == '>')
	{
		if (input[*i + 1] == '>')
		{
			*i += 2;
			return (create_tk(ms, TK_APPEND_OUTPUT, ">>"));
		}
		else
		{
			(*i)++;
			return (create_tk(ms, TK_REDIRECT_OUTPUT, ">"));
		}
	}
	else if (input[*i] == '<')
	{
		(*i)++;
		return (create_tk(ms, TK_REDIRECT_INPUT, "<"));
	}
	return (NULL);
}

/* create_pipe_tk()
 *	- if the current position is the start of the input (*i == 0), it returns
 *	NULL and displays a syntax error because '|' cannot be the first element
 *	of the input
 *	- advances the position in the input by incrementing (*i) to pass the '|'
 *	- returns a tk of type tk_PIPE with the value "|" or NULL in case of
 *	syntax error
 */
static t_tk	*create_pipe_tk(t_ms *ms, const char *input, int *i)
{
	(void)input;
	if (*i == 0)
	{
		syn_err(ms, "'|' can not be the first element of the cmd line");
		return (NULL);
	}
	(*i)++;
	return (create_tk(ms, TK_PIPE, "|"));
}

/* create_next_tk()
*	- determines the type of tk at the current position in the input string
*	- delegates tk creation and the advancement of the input index to the
*	appropriate function, returns the created tk which is NULL in case
*	of syntax error
* 	tk Types and Handlers:
*		- '|' -> create_pipe_tk()
*		- '<' or '>' -> create_redirection_tk()
*		- '\'' -> create_single_quote_tk()
*		- '"' -> create_double_quote_tk()
*		- any other character -> create_word_tk()	
*/
static t_tk	*create_next_tk(t_ms *ms, const char *input, int *i)
{
	if (input[*i] == '|')
		return (create_pipe_tk(ms, input, i));
	if (input[*i] == '<' || input[*i] == '>')
		return (create_redirection_tk(ms, input, i));
	if (input[*i] == '\'')
		return (create_single_quote_tk(ms, input, i));
	if (input[*i] == '"')
		return (create_double_quote_tk(ms, input, i));
	return (create_word_tk(ms, input, i));
}

/* tkize_input()
*	Processes the input string and converts it into an array of tks.
*	- skips over whithespace between each tk and assigns the next part
*	of the input into a tk
*	- each valid tk is added to the tk array
*	- stops the process if a syntax error occurs (return(-1))
*	or at the end of the input
*	- adds a NULL guard at the end of the tk array
*/
static int	tkize_input(t_ms *ms, t_tk **tks, const char *input)
{
	int		tk_index;
	int		i;
	t_tk	*tk;

	i = 0;
	tk_index = 0;
	while (input[i] != '\0')
	{
		skip_whitespace(input, &i);
		tk = create_next_tk(ms, input, &i);
		if (!tk)
			return (-1);
		tks[tk_index++] = tk;
	}
	tks[tk_index] = NULL;
	return (0);
}

/* initialize_tks()
*	- allocates memory for the tk array 't_tk **tks' and 
*	returns it
*/
static t_tk	**initialize_tks(t_ms *ms)
{
	t_tk	**tks;

	tks = malloc(sizeof(t_tk *) * MAX_TKS);
	if (!tks)
		error(ms, "Error malloc failed in initialize_tks()");
	return (tks);
}

/* lexical_analyser()
*	Parameters:
*	- 't_ms *ms' is only passed here and to the lower functions in
*	case of allocation memory error so all the allocated memory can be freed
*	- 'const char *input' is the raw_input coming from readline()
*	Responsabilities
*	- initializes a tk array 't_tk **tks'
*	- pass the process of tkization to tkize_input()
*	Returns:
*	- NULL only in case of syntax error
*	- the tk array 
*/
t_tk	**lexical_analyser(t_ms *ms, const char *input)
{
	t_tk	**tks;

	tks = initialize_tks(ms);
	if (tkize_input(ms, tks, input) == -1)
		return (NULL);
	return (tks);
}
