/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_op_tk.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nlouis <nlouis@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/13 01:07:01 by nlouis            #+#    #+#             */
/*   Updated: 2025/02/13 10:17:06 by nlouis           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	create_pipe_tk(t_ms *ms, int *i, int *tk_index)
{
	ms->tks[*tk_index] = new_tk(ms, TK_PIPE, "|", QUOTE_NONE);
	(*tk_index)++;
	(*i)++;
}

void	create_redir_input_tk(t_ms *ms, int *i, int *tk_index)
{
	ms->tks[*tk_index] = new_tk(ms, TK_REDIRECT_INPUT, "<", QUOTE_NONE);
	(*tk_index)++;
	(*i)++;
}

void	create_redir_output_tk(t_ms *ms, int *i, int *tk_index)
{
	ms->tks[*tk_index] = new_tk(ms, TK_REDIRECT_OUTPUT, ">", QUOTE_NONE);
	(*tk_index)++;
	(*i)++;
}

void	create_append_output_tk(t_ms *ms, int *i, int *tk_index)
{
	ms->tks[*tk_index] = new_tk(ms, TK_APPEND_OUTPUT, ">>", QUOTE_NONE);
	(*tk_index)++;
	(*i) += 2;
}

void	create_heredoc_tk(t_ms *ms, int *i, int *tk_index)
{
	ms->tks[*tk_index] = new_tk(ms, TK_HEREDOC, "<<", QUOTE_NONE);
	(*tk_index)++;
	(*i) += 2;
	skip_whitespace_index(ms->input, i);
	if (ms->input[*i] && ft_is_operator(ms->input, *i) == 0)
	{
		ms->tks[*tk_index] = create_delimiter_tk(ms, i);
		(*tk_index)++;
	}
}
