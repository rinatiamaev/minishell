/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   collapse_adjacent_str.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nlouis <nlouis@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/31 20:39:19 by nlouis            #+#    #+#             */
/*   Updated: 2025/02/04 14:34:24 by nlouis           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* parse_sq_seg():
**   - Skip the opening single quote.
**   - Copy everything literally until the next single quote or end-of-string.
**   - No expansions inside single quotes.
**   - Append the extracted literal to 'buffer'.
*/
void	collapse_sq_seg(t_collapse	*col)
{
	int		start;
	char	*literal;
	char	*tmp;

	(*col->i)++;
	start = *col->i;
	while (col->input[*col->i] && col->input[*col->i] != '\'')
		(*col->i)++;
	if (!col->input[*col->i])
	{
		syn_err(col->ms, "unclosed single quote");
		return ;
	}
	literal = ft_substr(col->input, start, *col->i - start);
	if (!literal)
		error(col->ms, "malloc failed in collapse_sq_seg");
	(*col->i)++;
	if (col->is_delimiter)
	{
		tmp = ft_strjoin_free(ft_strdup("'"), literal);
		literal = ft_strjoin_free(tmp, ft_strdup("'"));
	}
	*col->buffer = ft_strjoin_free(*col->buffer, literal);
}

/* parse_dq_seg():
**   - Skip the opening double quote.
**   - Copy everything until the closing double quote or end-of-string.
**   - We DO expansions here (like $VAR, $?).
**   - Append the expanded result to 'buffer'.
*/
void	collapse_dq_seg(t_collapse	*col)
{
	int		start;
	char	*raw_content;
	char	*expanded;

	(*col->i)++;
	start = *col->i;
	while (col->input[*col->i] && col->input[*col->i] != '"')
		(*col->i)++;
	if (!col->input[*col->i])
	{
		syn_err(col->ms, "unclosed double quote");
		return ;
	}
	raw_content = ft_substr(col->input, start, *col->i - start);
	if (!raw_content)
		error(col->ms, "malloc failed in collapse_dq_seg");
	(*col->i)++;
	expanded = expand_env_var(col->ms, raw_content);
	free(raw_content);
	*col->buffer = ft_strjoin_free(*col->buffer, expanded);
}

static bool	should_break_unquoted(const char *input, int *i)
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
void	collapse_uq_seg(t_collapse	*col)
{
	int		start;
	char	*raw_content;
	char	*expanded;

	start = *col->i;
	while (col->input[*col->i])
	{
		if (should_break_unquoted(col->input, col->i))
			break ;
		(*col->i)++;
	}
	if (start == *col->i)
		return ;
	raw_content = ft_substr(col->input, start, (*col->i - start));
	if (!raw_content)
		error(col->ms, "ft_substr failed in collapse_uq_seg");
	expanded = expand_env_var(col->ms, raw_content);
	free(raw_content);
	*col->buffer = ft_strjoin_free(*col->buffer, expanded);
}
