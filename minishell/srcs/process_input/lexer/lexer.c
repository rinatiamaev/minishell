/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nlouis <nlouis@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/02 16:20:10 by nlouis            #+#    #+#             */
/*   Updated: 2025/02/15 12:05:51 by nlouis           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
 * lex_operator:
 *   Processes an operator token from the input string.
 *   Determines the operator type and creates the corresponding token.
 */
static void	lex_operator(t_ms *ms, int *i, int *tk_index)
{
	int	op_len;

	op_len = ft_is_operator(ms->input, *i);
	if (ms->input[*i] == '|')
	{
		create_pipe_tk(ms, i, tk_index);
	}
	else if (ms->input[*i] == '<')
	{
		if (op_len == 2)
			create_heredoc_tk(ms, i, tk_index);
		else
			create_redir_input_tk(ms, i, tk_index);
	}
	else if (ms->input[*i] == '>')
	{
		if (op_len == 2)
			create_append_output_tk(ms, i, tk_index);
		else
			create_redir_output_tk(ms, i, tk_index);
	}
}

/*
 * lexer:
 *   Tokenizes the input string stored in ms->input.
 *   Allocates an array for tokens and processes the input by skipping
 *   whitespace, handling operators, and creating word tokens.
 *   Returns 0 on success, or -1 on failure.
 */
int	lexer(t_ms *ms)
{
	int		i;
	int		tk_index;

	ms->tks = ft_calloc(MAX_TKS, sizeof(t_tk *));
	if (!ms->tks)
		error(ms, "malloc() failed in initialize_tks()");
	i = 0;
	tk_index = 0;
	while (ms->input[i])
	{
		skip_whitespace_index(ms->input, &i);
		if (!ms->input[i])
			break ;
		if (ft_is_operator(ms->input, i) > 0)
			lex_operator(ms, &i, &tk_index);
		else
		{
			ms->tks[tk_index] = create_word_tk(ms, &i, tk_index);
			if (!ms->tks[tk_index])
				return (-1);
			tk_index++;
		}
	}
	ms->tks[tk_index] = NULL;
	return (0);
}
