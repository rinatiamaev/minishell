/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_var_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nlouis <nlouis@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/28 09:41:40 by nlouis            #+#    #+#             */
/*   Updated: 2025/02/15 13:12:16 by nlouis           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
 * get_env_variable:
 *   Searches ms->envp for an environment variable matching var_name.
 *   If found, returns a duplicate of its value (the substring after '=');
 *   otherwise, returns a duplicate of an empty string.
 */
static char	*get_env_variable(t_ms *ms, char *var_name)
{
	size_t	name_len;
	char	**env;

	name_len = ft_strlen(var_name);
	env = ms->envp;
	while (*env)
	{
		if (ft_strncmp(*env, var_name, name_len) == 0 \
			&& (*env)[name_len] == '=')
			return (x_strdup(ms, *env + name_len + 1));
		env++;
	}
	return (x_strdup(ms, ""));
}

/*
 * expand_env_variable:
 *   Expands an environment variable by obtaining its value using
 *   get_env_variable. If no value is found, returns an empty string.
 */
static char	*expand_env_variable(t_ms *ms, char *content)
{
	char	*value;

	value = get_env_variable(ms, content);
	if (!value)
		value = x_strdup(ms, "");
	return (value);
}

/*
 * expand_exit_status:
 *   Converts the current exit status to a string.
 *   Uses ft_itoa to convert the integer exit_status into a string.
 */
static char	*expand_exit_status(t_ms *ms, int exit_status)
{
	char	*status;

	status = ft_itoa(exit_status);
	if (!status)
		error(ms, "ft_itoa() failed in expand_exit_status()");
	return (status);
}

/*
 * process_variable:
 *   Processes a variable expansion starting at content[*i].
 *   If the variable is "$?" it expands to the exit status.
 *   If no valid variable name follows '$', returns "$" as literal.
 *   Otherwise, extracts the variable name, expands it, and returns
 *   the corresponding value.
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
	if (content[*i] == '\0')
		return (x_strdup(ms, "$"));
	if (!ft_isalnum(content[*i]) && content[*i] != '_')
		return (x_strdup(ms, "$"));
	while (content[*i] && (ft_isalnum_underscore(content[*i])))
		(*i)++;
	var_name = x_substr(ms, content, start + 1, (*i - (start + 1)));
	if (!var_name || var_name[0] == '\0')
		return (free(var_name), x_strdup(ms, "$"));
	expanded = expand_env_variable(ms, var_name);
	free(var_name);
	return (expanded);
}

/*
 * process_literal:
 *   Processes a literal segment from content starting at *i until a '$'
 *   is encountered or end of string is reached.
 *   Returns a substring representing the literal segment.
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
