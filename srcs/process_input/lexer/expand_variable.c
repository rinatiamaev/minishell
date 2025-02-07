/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_variable.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nlouis <nlouis@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/28 09:36:53 by nlouis            #+#    #+#             */
/*   Updated: 2025/02/07 11:51:40 by nlouis           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
