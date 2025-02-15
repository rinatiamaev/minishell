/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_op_tk.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nlouis <nlouis@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/13 01:07:01 by nlouis            #+#    #+#             */
/*   Updated: 2025/02/15 12:14:24 by nlouis           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
 * create_pipe_tk:
 *   Creates a token for a pipe operator ("|"), updates the token index
 *   and advances the input index by 1.
 */
void	create_pipe_tk(t_ms *ms, int *i, int *tk_index)
{
	ms->tks[*tk_index] = new_tk(ms, TK_PIPE, "|", QUOTE_NONE);
	(*tk_index)++;
	(*i)++;
}

/*
 * create_redir_input_tk:
 *   Creates a token for an input redirection operator ("<"), updates
 *   the token index and advances the input index by 1.
 */
void	create_redir_input_tk(t_ms *ms, int *i, int *tk_index)
{
	ms->tks[*tk_index] = new_tk(ms, TK_REDIRECT_INPUT, "<", QUOTE_NONE);
	(*tk_index)++;
	(*i)++;
}

/*
 * create_redir_output_tk:
 *   Creates a token for an output redirection operator (">"), updates
 *   the token index and advances the input index by 1.
 */
void	create_redir_output_tk(t_ms *ms, int *i, int *tk_index)
{
	ms->tks[*tk_index] = new_tk(ms, TK_REDIRECT_OUTPUT, ">", QUOTE_NONE);
	(*tk_index)++;
	(*i)++;
}

/*
 * create_append_output_tk:
 *   Creates a token for an append output redirection operator (">>"),
 *   updates the token index and advances the input index by 2.
 */
void	create_append_output_tk(t_ms *ms, int *i, int *tk_index)
{
	ms->tks[*tk_index] = new_tk(ms, TK_APPEND_OUTPUT, ">>", QUOTE_NONE);
	(*tk_index)++;
	(*i) += 2;
}

/*
 * create_heredoc_tk:
 *   Creates a token for a heredoc operator ("<<"), updates the token
 *   index and advances the input index by 2. It then skips any
 *   whitespace and, if the next character is not an operator,
 *   creates a delimiter token.
 */
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
