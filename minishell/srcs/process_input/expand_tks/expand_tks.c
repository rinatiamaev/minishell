/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_tks.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nlouis <nlouis@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/13 12:58:45 by nlouis            #+#    #+#             */
/*   Updated: 2025/02/13 12:58:46 by nlouis           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*expand_in_tk(t_ms *ms, char *str)
{
	char	*result;
	char	*tmp;
	char	*processed;
	int		i;

	result = x_strdup(ms, "");
	i = 0;
	while (str[i] != '\0')
	{
		if (str[i] == '$')
			processed = process_variable(ms, (char *)str, &i);
		else
			processed = process_literal(ms, (char *)str, &i);
		tmp = x_strjoin_free(ms, result, processed);
		result = tmp;
	}
	return (result);
}

int	expand_tks(t_ms *ms)
{
	int		i;
	t_tk	*tk;
	char	*expanded;

	i = 0;
	while (ms->tks[i])
	{
		tk = ms->tks[i];
		if (tk->type == TK_WORD)
		{
			if (tk->qs != QUOTE_SINGLE)
			{
				expanded = expand_in_tk(ms, tk->value);
				free(tk->value);
				tk->value = expanded;
			}
		}
		i++;
	}
	return (0);
}
