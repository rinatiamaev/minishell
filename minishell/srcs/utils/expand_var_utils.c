/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_variable_utils.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nlouis <nlouis@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/28 09:41:40 by nlouis            #+#    #+#             */
/*   Updated: 2025/02/13 00:51:56 by nlouis           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

static char	*expand_env_variable(t_ms *ms, char *content)
{
	char	*value;

	value = get_env_variable(ms, content);
	if (!value)
		value = x_strdup(ms, "");
	return (value);
}

static char	*expand_exit_status(t_ms *ms, int exit_status)
{
	char	*status;

	status = ft_itoa(exit_status);
	if (!status)
		error(ms, "ft_itoa() failed in expand_exit_status()");
	return (status);
}

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
