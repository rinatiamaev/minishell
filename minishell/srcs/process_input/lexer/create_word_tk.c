/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_word_tk.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nlouis <nlouis@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/13 01:08:52 by nlouis            #+#    #+#             */
/*   Updated: 2025/02/14 14:40:23 by nlouis           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	gather_word_segments(t_ms *ms, int *i, int tk_index)
{
	int	old_i;

	while (ms->input[*i])
	{
		if (ms->input[*i] == '\'')
		{
			if (collapse_sq_seg(ms, i, tk_index) == -1)
				return (-1);
		}
		else if (ms->input[*i] == '"')
		{
			if (collapse_dq_seg(ms, i, tk_index) == -1)
				return (-1);
		}
		else
		{
			old_i = *i;
			collapse_uq_seg(ms, i, tk_index);
			if (old_i == *i)
				break ;
		}
		if (!ms->input[*i] || ft_isspace_operator(ms->input, *i))
			break ;
	}
	return (0);
}

t_tk	*create_word_tk(t_ms *ms, int *i, int tk_index)
{
	ms->tks[tk_index] = new_tk(ms, TK_WORD, "", QUOTE_NONE);
	if (gather_word_segments(ms, i, tk_index) == -1)
	{
		if (ms->tks[tk_index])
		{
			free(ms->tks[tk_index]->value);
			free(ms->tks[tk_index]);
			ms->tks[tk_index] = NULL;
		}
		return (NULL);
	}
	return (ms->tks[tk_index]);
}
