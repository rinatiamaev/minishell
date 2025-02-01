/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nlouis <nlouis@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/02 16:20:10 by nlouis            #+#    #+#             */
/*   Updated: 2025/02/01 18:56:09 by nlouis           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*collapse_token(t_ms *ms, const char *input, int *i)
{
	char	*word;

	word = ft_strdup("");
	if (!word)
		error(ms, "malloc failed in collapse_token");
	while (input[*i])
	{
		if (ft_isspace(input[*i]) || ft_is_operator(input, *i) > 0)
			break ;
		if (input[*i] == '\'')
			collapse_sq_seg(ms, input, i, &word);
		else if (input[*i] == '"')
			collapse_dq_seg(ms, input, i, &word);
		else
			collapse_uq_seg(ms, input, i, &word);
	}
	return (word);
}

static t_tk	*create_op_tk(t_ms *ms, const char *input, int *i, int op_len)
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

static t_tk	*create_next_tk(t_ms *ms, const char *input, int *i)
{
	t_tk	*tk;
	int		op_len;
	char	*word;

	skip_whitespace_index(input, i);
	if (input[*i] == '\0')
		return (NULL);
	op_len = ft_is_operator(input, *i);
	if (op_len > 0)
	{
		tk = create_op_tk(ms, input, i, op_len);
		return (tk);
	}
	else
	{
		word = collapse_token(ms, input, i);
		if (!word || !*word)
		{
			free(word);
			return (NULL);
		}
		tk = create_tk(ms, TK_WORD, word);
		free(word);
		return (tk);
	}
}

static int	tkize_input(t_ms *ms, t_tk **tks, const char *input)
{
	int		i;
	int		tk_index;
	t_tk	*tk;

	i = 0;
	tk_index = 0;
	tks[tk_index] = NULL;
	while (input[i] != '\0')
	{
		skip_whitespace_index(input, &i);
		if (input[i] == '\0')
			break ;
		tk = create_next_tk(ms, input, &i);
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
