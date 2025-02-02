/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_tokens.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nlouis <nlouis@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/28 09:30:35 by nlouis            #+#    #+#             */
/*   Updated: 2025/02/02 14:10:33 by nlouis           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_tk	*create_pipe_tk(t_ms *ms, int *i)
{
	(*i)++;
	return (create_tk(ms, TK_PIPE, "|"));
}

t_tk	*create_heredoc_tk(t_ms *ms, int *i)
{
	*i += 2;
	return (create_tk(ms, TK_HEREDOC, "<<"));
}

t_tk	*create_redirect_input_tk(t_ms *ms, int *i)
{
	(*i)++;
	return (create_tk(ms, TK_REDIRECT_INPUT, "<"));
}

t_tk	*create_redirect_output_tk(t_ms *ms, int *i)
{
	(*i)++;
	return (create_tk(ms, TK_REDIRECT_OUTPUT, ">"));
}

t_tk	*create_append_output_tk(t_ms *ms, int *i)
{
	*i += 2;
	return (create_tk(ms, TK_APPEND_OUTPUT, ">>"));
}
