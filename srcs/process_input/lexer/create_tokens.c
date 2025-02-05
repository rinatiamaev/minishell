/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_tokens.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nlouis <nlouis@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/28 09:30:35 by nlouis            #+#    #+#             */
/*   Updated: 2025/02/05 11:55:36 by nlouis           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	create_pipe_tk(t_ms *ms, int *i, int tk_index)
{
	(*i)++;
	create_tk(ms, tk_index, TK_PIPE, "|");
}

void	create_heredoc_tk(t_ms *ms, int *i, int tk_index)
{
	*i += 2;
	create_tk(ms, tk_index, TK_HEREDOC, "<<");
}

void	create_redirect_input_tk(t_ms *ms, int *i, int tk_index)
{
	(*i)++;
	create_tk(ms, tk_index, TK_REDIRECT_INPUT, "<");
}

void	create_redirect_output_tk(t_ms *ms, int *i, int tk_index)
{
	(*i)++;
	create_tk(ms, tk_index, TK_REDIRECT_OUTPUT, ">");
}

void	create_append_output_tk(t_ms *ms, int *i, int tk_index)
{
	*i += 2;
	create_tk(ms, tk_index, TK_APPEND_OUTPUT, ">>");
}
