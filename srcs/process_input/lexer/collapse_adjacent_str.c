/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   collapse_adjacent_str.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nlouis <nlouis@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/31 20:39:19 by nlouis            #+#    #+#             */
/*   Updated: 2025/02/03 12:44:39 by nlouis           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* parse_sq_seg():
**   - Skip the opening single quote.
**   - Copy everything literally until the next single quote or end-of-string.
**   - No expansions inside single quotes.
**   - Append the extracted literal to 'buffer'.
*/
void	collapse_sq_seg(t_ms *ms, const char *input, int *i, char **buffer, bool is_heredoc)
{
	int		start;
	char	*literal;
	char	*temp;
	char	*joined;

	(*i)++;
	start = *i;
	while (input[*i] && input[*i] != '\'')
		(*i)++;
	if (!input[*i])
	{
		syn_err(ms, "unclosed single quote");
		return ;
	}
	literal = ft_substr(input, start, *i - start);
	if (!literal)
		error(ms, "malloc failed in parse_sq_seg");
	(*i)++;
	if (is_heredoc)
	{
		temp = ft_strjoin(literal, "'");
		free(literal);
		literal = ft_strjoin("'", temp);
		free(temp);
	}
	joined = ft_strjoin(*buffer, literal);
	if (!joined)
		error(ms, "malloc failed in parse_sq_seg (join)");
	free(*buffer);
	free(literal);
	*buffer = joined;
}

static void	append_and_replace_buffer(t_ms *ms, char **buffer, char *new_part)
{
	char	*joined;

	joined = ft_strjoin(*buffer, new_part);
	if (!joined)
		error(ms, "ft_strjoin() failed in append_and_replace_buffer");
	free(*buffer);
	free(new_part);
	*buffer = joined;
}

/* parse_dq_seg():
**   - Skip the opening double quote.
**   - Copy everything until the closing double quote or end-of-string.
**   - We DO expansions here (like $VAR, $?).
**   - Append the expanded result to 'buffer'.
*/
void	collapse_dq_seg(t_ms *ms, const char *input, int *i, char **buffer)
{
	int		start;
	char	*raw_content;
	char	*expanded;

	(*i)++;
	start = *i;
	while (input[*i] && input[*i] != '"')
		(*i)++;
	if (!input[*i])
	{
		syn_err(ms, "unclosed double quote");
		return ;
	}
	raw_content = ft_substr(input, start, *i - start);
	if (!raw_content)
		error(ms, "malloc failed in parse_dq_seg");
	(*i)++;
	expanded = expand_env_var(ms, raw_content);
	free(raw_content);
	append_and_replace_buffer(ms, buffer, expanded);
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
void	collapse_uq_seg(t_ms *ms, const char *input, int *i, char **buffer)
{
	int		start;
	char	*raw_content;
	char	*expanded;

	start = *i;
	while (input[*i])
	{
		if (should_break_unquoted(input, i) == true)
			break ;
		(*i)++;
	}
	if (start == *i)
		return ;
	raw_content = ft_substr(input, start, (*i - start));
	if (!raw_content)
		error(ms, "ft_substr failed in parse_uq_seg");
	expanded = expand_env_var(ms, raw_content);
	free(raw_content);
	append_and_replace_buffer(ms, buffer, expanded);
}
