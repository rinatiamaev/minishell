/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nlouis <nlouis@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/31 20:21:20 by nlouis            #+#    #+#             */
/*   Updated: 2025/02/13 10:16:19 by nlouis           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_tk	*new_tk(t_ms *ms, t_tk_type type, const char *value, t_qs qs)
{
	t_tk	*tk;

	tk = x_malloc(ms, sizeof(t_tk));
	tk->type = type;
	tk->qs = qs;
	tk->delimiter_quoted = false;
	if (value)
		tk->value = x_strdup(ms, value);
	else
		tk->value = x_strdup(ms, "");
	return (tk);
}

int	ft_is_operator(char *input, int i)
{
	if (input[i] == '|')
		return (1);
	if (input[i] == '<')
	{
		if (input[i + 1] == '<')
			return (2);
		return (1);
	}
	if (input[i] == '>')
	{
		if (input[i + 1] == '>')
			return (2);
		return (1);
	}
	return (0);
}

int	ft_isspace_operator(char *input, int i)
{
	return (ft_isspace(input[i]) || ft_is_operator(input, i));
}
