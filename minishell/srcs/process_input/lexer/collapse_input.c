/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   collapse_input.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nlouis <nlouis@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/13 00:35:43 by nlouis            #+#    #+#             */
/*   Updated: 2025/02/15 12:23:17 by nlouis           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
 * collapse_sq_seg:
 *   Processes a single-quoted segment in the input. Advances the index
 *   past the closing quote, appends the literal string to the token's
 *   value, and sets the token's quote status to QUOTE_SINGLE. Returns -1
 *   if no closing quote is found.
 */
int	collapse_sq_seg(t_ms *ms, int *i, int tk_index)
{
	int		start;
	char	*literal;
	char	*tmp;

	(*i)++;
	start = *i;
	while (ms->input[*i] && ms->input[*i] != '\'')
		(*i)++;
	if (!ms->input[*i])
	{
		syn_err(ms, "unclosed single quote");
		return (-1);
	}
	literal = x_substr(ms, ms->input, start, (*i - start));
	(*i)++;
	tmp = x_strjoin_free(ms, ms->tks[tk_index]->value, literal);
	ms->tks[tk_index]->value = tmp;
	ms->tks[tk_index]->qs = QUOTE_SINGLE;
	return (0);
}

/*
 * collapse_dq_seg:
 *   Processes a double-quoted segment in the input. Advances the index
 *   past the closing quote, appends the raw content to the token's value,
 *   and sets the token's quote status to QUOTE_DOUBLE. Returns -1 if no
 *   closing quote is found.
 */
int	collapse_dq_seg(t_ms *ms, int *i, int tk_index)
{
	int		start;
	char	*raw_content;
	char	*tmp;

	(*i)++;
	start = *i;
	while (ms->input[*i] && ms->input[*i] != '"')
		(*i)++;
	if (!ms->input[*i])
	{
		syn_err(ms, "unclosed double quote");
		return (-1);
	}
	raw_content = x_substr(ms, ms->input, start, (*i - start));
	(*i)++;
	tmp = x_strjoin_free(ms, ms->tks[tk_index]->value, raw_content);
	ms->tks[tk_index]->value = tmp;
	ms->tks[tk_index]->qs = QUOTE_DOUBLE;
	return (0);
}

/*
 * should_break_unquoted:
 *   Checks if the current character in the input should break an unquoted
 *   segment. Returns true if the character is a whitespace, a quote, an
 *   operator.
 */
static bool	should_break_unquoted(char *input, int *i)
{
	if (ft_isspace(input[*i]))
		return (true);
	if (input[*i] == '\'' || input[*i] == '"')
		return (true);
	if (ft_is_operator(input, *i) > 0)
		return (true);
	return (false);
}

/*
 * collapse_uq_seg:
 *   Processes an unquoted segment in the input. Advances the index until a
 *   breaking character is encountered, and appends the segment to the token's
 *   value. Sets the token's quote status to QUOTE_NONE.
 */
void	collapse_uq_seg(t_ms *ms, int *i, int tk_index)
{
	int		start;
	char	*raw_content;
	char	*tmp;

	start = *i;
	while (ms->input[*i])
	{
		if (should_break_unquoted(ms->input, i))
			break ;
		(*i)++;
	}
	if (start == *i)
		return ;
	raw_content = x_substr(ms, ms->input, start, (*i - start));
	tmp = x_strjoin_free(ms, ms->tks[tk_index]->value, raw_content);
	ms->tks[tk_index]->value = tmp;
	ms->tks[tk_index]->qs = QUOTE_NONE;
}
