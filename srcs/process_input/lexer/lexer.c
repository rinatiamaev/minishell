/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nlouis <nlouis@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/02 16:20:10 by nlouis            #+#    #+#             */
/*   Updated: 2025/02/04 14:31:07 by nlouis           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*collapse_token(t_ms *ms, const char *input, int *i, t_tk *tk)
{
	char		*word;
	t_collapse	col;

	word = ft_strdup("");
	if (!word)
		error(ms, "malloc failed in collapse_token");
	col.ms = ms;
	col.input = input;
	col.i = i;
	col.buffer = &word;
	col.is_delimiter = tk->is_delimiter;
	while (col.input[*i])
	{
		if (ft_isspace(col.input[*i]) || ft_is_operator(col.input, *i) > 0)
			break ;
		if (col.input[*i] == '\'')
			collapse_sq_seg(&col);
		else if (col.input[*i] == '"')
			collapse_dq_seg(&col);
		else
			collapse_uq_seg(&col);
	}
	return (word);
}

static t_tk	*create_op_tk(t_ms *ms, const char *input, \
			int *i, int op_len)
{
	if (input[*i] == '|')
		return (create_pipe_tk(ms, i));
	else if (input[*i] == '<')
	{
		if (op_len == 2)
			return (create_heredoc_tk(ms, i));
		else
			return (create_redirect_input_tk(ms, i));
	}
	else if (input[*i] == '>')
	{
		if (op_len == 2)
			return (create_append_output_tk(ms, i));
		else
			return (create_redirect_output_tk(ms, i));
	}
	return (NULL);
}

static t_tk	*create_next_tk(t_ms *ms, const char *input, int *i, t_tk *p_token)
{
	t_tk	*tk;
	int		op_len;

	skip_whitespace_index(input, i);
	if (input[*i] == '\0')
		return (NULL);
	op_len = ft_is_operator(input, *i);
	if (op_len > 0)
		tk = create_op_tk(ms, input, i, op_len);
	else
	{
		tk = create_tk(ms, TK_WORD, NULL);
		tk->is_delimiter = (p_token && p_token->type == TK_HEREDOC);
		tk->value = collapse_token(ms, input, i, tk);
		if (tk->is_delimiter)
			detect_quoted_delimiter(tk);
	}
	return (tk);
}

static int	tkize_input(t_ms *ms, t_tk **tks, const char *input)
{
	int		i;
	int		tk_index;
	t_tk	*tk;
	t_tk	*prev_tk;

	i = 0;
	tk_index = 0;
	while (input[i] != '\0')
	{
		skip_whitespace_index(input, &i);
		if (input[i] == '\0')
			break ;
		if (tk_index == 0)
			prev_tk = NULL;
		else
			prev_tk = tks[tk_index - 1];
		tk = create_next_tk(ms, input, &i, prev_tk);
		if (!tk)
			return (-1);
		tks[tk_index++] = tk;
		tks[tk_index] = NULL;
	}
	tks[tk_index] = NULL;
	return (0);
}

t_tk	**lexer(t_ms *ms, const char *input)
{
	t_tk	**tks;

	tks = initialize_tks(ms);
	if (tkize_input(ms, tks, input) == -1)
		return (NULL);
	return (tks);
}
