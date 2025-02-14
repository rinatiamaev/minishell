/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   collapse.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nlouis <nlouis@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/13 00:35:43 by nlouis            #+#    #+#             */
/*   Updated: 2025/02/13 00:35:44 by nlouis           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

static bool	should_break_unquoted(char *input, int *i)
{
	if (ft_isspace(input[*i]))
		return (true);
	if (input[*i] == '\'' || input[*i] == '"')
		return (true);
	if (ft_is_operator(input, *i) > 0)
		return (true);
	if (input[*i] == '\\')
		return (true);
	return (false);
}

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
