/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_variable.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nlouis <nlouis@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/28 09:36:53 by nlouis            #+#    #+#             */
/*   Updated: 2025/02/01 19:00:55 by nlouis           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* expand_env_in_word()
 *	- handles basic environment variable expansion for unquoted words
 *	- if the entire string is "$?", expands it to the shell's last exit status
 *	- if the string starts with '$' followed by a non-null character, expands
 *   the variable via expand_env_variable()
 *	- otherwise, returns a copy of the original content
 *	- returns the expanded string on success
 */
char	*expand_env_var(t_ms *ms, const char *content)
{
	char	*output;
	char	*tmp;
	char	*processed;
	int		i;

	output = ft_strdup("");
	if (!output)
		error(ms, "Error ft_strdup() failed in expand_env_in_word");
	i = 0;
	while (content[i] != '\0')
	{
		if (content[i] == '$' && content[i + 1] != '\0')
			processed = process_variable(ms, content, &i);
		else
			processed = process_literal(ms, content, &i);
		tmp = ft_strjoin(output, processed);
		if (!tmp)
			error(ms, "Error ft_strjoin() failed in expand_env_in_word");
		free(processed);
		free(output);
		output = tmp;
	}
	return (output);
}
