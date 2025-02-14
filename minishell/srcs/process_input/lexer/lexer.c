/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nlouis <nlouis@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/02 16:20:10 by nlouis            #+#    #+#             */
/*   Updated: 2025/02/14 14:37:54 by nlouis           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
