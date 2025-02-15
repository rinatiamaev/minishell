/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nlouis <nlouis@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/31 20:21:20 by nlouis            #+#    #+#             */
/*   Updated: 2025/02/15 12:07:49 by nlouis           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
 * new_tk:
 *   Creates a new token with a given type, value, and quote status.
 *   It allocates memory for the token, sets its type and quote flag,
 *   and duplicates the provided value. If value is NULL, it duplicates
 *   an empty string.
 */
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

/*
 * ft_is_operator:
 *   Checks if the character at index i in input is an operator.
 *   Returns 1 for a single operator ('|', '<', '>') and 2 for a
 *   double operator ("<<" or ">>"). Returns 0 if not an operator.
 */
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
