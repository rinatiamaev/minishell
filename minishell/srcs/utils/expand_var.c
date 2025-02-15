/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_var.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nlouis <nlouis@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/28 09:36:53 by nlouis            #+#    #+#             */
/*   Updated: 2025/02/15 13:11:09 by nlouis           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
 * expand_env_var:
 *   Expands all environment variables within the given content string.
 *   It processes the string character by character. When a '$'
 *   is encountered, process_variable() is called to expand the
 *   variable. Otherwise, process_literal() processes a literal
 *   segment. The resulting strings are concatenated to form the final
 *   expanded output.
 */
char	*expand_env_var(t_ms *ms, char *content)
{
	char	*output;
	char	*tmp;
	char	*processed;
	int		i;

	output = x_strdup(ms, "");
	i = 0;
	while (content[i] != '\0')
	{
		if (content[i] == '$')
			processed = process_variable(ms, content, &i);
		else
			processed = process_literal(ms, content, &i);
		tmp = x_strjoin_free(ms, output, processed);
		output = tmp;
	}
	return (output);
}
