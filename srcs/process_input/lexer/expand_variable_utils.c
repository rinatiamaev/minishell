/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_variable_utils.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nlouis <nlouis@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/28 09:41:40 by nlouis            #+#    #+#             */
/*   Updated: 2025/02/05 12:52:17 by nlouis           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

static char	*get_env_variable(t_ms *ms, char *var_name)
{
	size_t	name_len;
	char	**env;

	name_len = ft_strlen(var_name);
	env = ms->envp;
	while (*env)
	{
		if (ft_strncmp(*env, var_name, name_len) == 0
			&& (*env)[name_len] == '=')
			return (x_strdup(ms, *env + name_len + 1));
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

char	*expand_env_variable(t_ms *ms, char *content)
{
	char	*value;

	value = get_env_variable(ms, content);
	if (!value)
		value = x_strdup(ms, "");
	return (value);
}

/* expand_exit_status()
 *	- converts the given exit status to a string using ft_itoa.
 *	- returns the string or exits on memory error.
 */
char	*expand_exit_status(t_ms *ms, int exit_status)
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

char	*process_variable(t_ms *ms, char *content, int *i)
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
	var_name = x_substr(ms, content, start + 1, (*i - (start + 1)));
	expanded = expand_env_variable(ms, var_name);
	free(var_name);
	return (expanded);
}

/* process_literal()
 *	- accumulates literal characters in 'word' until encountering a '$' or '\0'
 *	- returns the substring
 */
char	*process_literal(t_ms *ms, char *content, int *i)
{
	int		start;
	char	*literal;

	start = *i;
	while (content[*i] != '\0' && content[*i] != '$')
		(*i)++;
	literal = x_substr(ms, content, start, *i - start);
	return (literal);
}
