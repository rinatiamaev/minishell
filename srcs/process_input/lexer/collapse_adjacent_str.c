/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   collapse_adjacent_str.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nlouis <nlouis@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/31 20:39:19 by nlouis            #+#    #+#             */
/*   Updated: 2025/02/05 13:02:01 by nlouis           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* parse_sq_seg():
**   - Skip the opening single quote.
**   - Copy everything literally until the next single quote or end-of-string.
**   - No expansions inside single quotes.
**   - Append the extracted literal to 'buffer'.
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
	literal = x_substr(ms, ms->input, start, *i - start);
	(*i)++;
	if (ms->tks[tk_index]->type == TK_HEREDOC_DELIMITER)
	{
		tmp = x_strjoin_free(ms, x_strdup(ms, "'"), literal);
		literal = x_strjoin_free(ms, tmp, x_strdup(ms, "'"));
	}
	ms->tks[tk_index]->value
		= x_strjoin_free(ms, ms->tks[tk_index]->value, literal);
	return (0);
}

/* parse_dq_seg():
**   - Skip the opening double quote.
**   - Copy everything until the closing double quote or end-of-string.
**   - We DO expansions here (like $VAR, $?).
**   - Append the expanded result to 'buffer'.
*/
int	collapse_dq_seg(t_ms *ms, int *i, int tk_index)
{
	int		start;
	char	*raw_content;
	char	*expanded;

	(*i)++;
	start = *i;
	while (ms->input[*i] && ms->input[*i] != '"')
		(*i)++;
	if (!ms->input[*i])
	{
		syn_err(ms, "unclosed double quote");
		return (-1);
	}
	raw_content = x_substr(ms, ms->input, start, *i - start);
	(*i)++;
	expanded = expand_env_var(ms, raw_content);
	free(raw_content);
	ms->tks[tk_index]->value
		= x_strjoin_free(ms, ms->tks[tk_index]->value, expanded);
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
	{
		(*i)++;
		return (true);
	}
	return (false);
}

/* parse_uq_seg():
**   - Collect characters until we hit whitespace, quotes, operator, or NUL.
**   - Expansions ($VAR, $?) are performed on the unquoted segment.
**   - Append the expanded text to 'buffer'.
*/
void	collapse_uq_seg(t_ms *ms, int *i, int tk_index)
{
	int		start;
	char	*raw_content;
	char	*expanded;

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
	expanded = expand_env_var(ms, raw_content);
	free(raw_content);
	ms->tks[tk_index]->value
		= x_strjoin_free(ms, ms->tks[tk_index]->value, expanded);
}
