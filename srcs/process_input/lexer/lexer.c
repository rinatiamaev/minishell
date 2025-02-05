/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nlouis <nlouis@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/02 16:20:10 by nlouis            #+#    #+#             */
/*   Updated: 2025/02/05 14:05:49 by nlouis           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	collapse_token(t_ms *ms, int *i, int tk_index)
{
	ms->tks[tk_index]->value = ft_strdup("");
	if (!ms->tks[tk_index]->value)
		error(ms, "malloc failed in collapse_token");
	while (ms->input[*i])
	{
		if (ft_isspace(ms->input[*i]) || ft_is_operator(ms->input, *i) > 0)
			break ;
		if (ms->input[*i] == '\'')
		{
			if (collapse_sq_seg(ms, i, tk_index))
				return (-1);
		}
		else if (ms->input[*i] == '"')
		{
			if (collapse_dq_seg(ms, i, tk_index))
				return (-1);
		}
		else
		{
			collapse_uq_seg(ms, i, tk_index);
		}
	}
	return (0);
}

static void	create_op_tk(t_ms *ms, int tk_index, int *i, int op_len)
{
	if (ms->input[*i] == '|')
		create_pipe_tk(ms, i, tk_index);
	else if (ms->input[*i] == '<')
	{
		if (op_len == 2)
			create_heredoc_tk(ms, i, tk_index);
		else
			create_redirect_input_tk(ms, i, tk_index);
	}
	else if (ms->input[*i] == '>')
	{
		if (op_len == 2)
			create_append_output_tk(ms, i, tk_index);
		else
			create_redirect_output_tk(ms, i, tk_index);
	}
}

static int	create_next_tk(t_ms *ms, int *i, int tk_index)
{
	int	op_len;

	skip_whitespace_index(ms->input, i);
	if (ms->input[*i] == '\0')
	{
		ms->tks[tk_index] = NULL;
		return (-1);
	}
	op_len = ft_is_operator(ms->input, *i);
	if (op_len > 0)
		create_op_tk(ms, tk_index, i, op_len);
	else
	{
		if (tk_index > 0 && ms->tks[tk_index - 1]->type == TK_HEREDOC)
			create_tk(ms, tk_index, TK_HEREDOC_DELIMITER, NULL);
		else
			create_tk(ms, tk_index, TK_WORD, NULL);
		if (collapse_token(ms, i, tk_index))
			return (-1);
		if (ms->tks[tk_index]->type == TK_HEREDOC_DELIMITER)
			detect_quoted_delimiter(ms->tks[tk_index]);
	}
	return (0);
}

static int	tkize_input(t_ms *ms)
{
	int		i;
	int		tk_index;

	i = 0;
	tk_index = 0;
	while (ms->input[i] != '\0')
	{
		skip_whitespace_index(ms->input, &i);
		if (ms->input[i] == '\0')
			break ;
		if (create_next_tk(ms, &i, tk_index) == -1)
			return (-1);
		if (!ms->tks[tk_index] || !ms->tks[tk_index]->value)
			return (-1);
		tk_index++;
	}
	ms->tks[tk_index] = NULL;
	return (0);
}

int	lexer(t_ms *ms)
{
	ms->tks = ft_calloc(MAX_TKS, sizeof(t_tk *));
	if (!ms->tks)
		error(ms, "malloc() failed in initialize_tks()");
	if (tkize_input(ms))
		return (-1);
	return (0);
}
