/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_delim_tk.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nlouis <nlouis@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/13 01:06:52 by nlouis            #+#    #+#             */
/*   Updated: 2025/02/13 10:16:19 by nlouis           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	detect_quoted_delimiter(t_ms *ms, t_tk *tk)
{
	int		len;
	char	*cleaned;

	if (!tk->value)
		return ;
	len = ft_strlen(tk->value);
	if (len < 2)
		return ;
	if ((tk->value[0] == '\'' && tk->value[len - 1] == '\'')
		|| (tk->value[0] == '"' && tk->value[len - 1] == '"'))
	{
		tk->delimiter_quoted = true;
		cleaned = x_substr(ms, tk->value, 1, len - 2);
		if (cleaned)
		{
			free(tk->value);
			tk->value = cleaned;
		}
	}
}

t_tk	*create_delimiter_tk(t_ms *ms, int *i)
{
	t_tk	*tk;
	int		start;
	char	*raw_delim;

	tk = new_tk(ms, TK_HEREDOC_DELIMITER, "", QUOTE_NONE);
	start = *i;
	while (ms->input[*i] && !ft_isspace(ms->input[*i])
		&& ft_is_operator(ms->input, *i) == 0)
		(*i)++;
	raw_delim = x_substr(ms, ms->input, start, (*i - start));
	free(tk->value);
	tk->value = raw_delim;
	detect_quoted_delimiter(ms, tk);
	return (tk);
}
