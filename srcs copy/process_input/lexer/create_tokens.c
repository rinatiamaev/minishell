/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_tokens.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nlouis <nlouis@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/28 09:30:35 by nlouis            #+#    #+#             */
/*   Updated: 2025/01/28 09:40:00 by nlouis           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* create_double_quote_tk()
 *	- skips the opening double quote by advancing the index (*i)
 *	- identifies the starting point of the content within the double
 *	quotes in start
 *	- handles unclosed double quotes by reporting a syntax error and
 *	returning NULL
 *	- extracts the content between the quotes using `ft_substr`
 *	- expands environment variables within the double-quoted content
 *	- creates a tk of type `tk_WORD` with the expanded content
 *	- handles memory allocation errors for the raw content
 *	- returns the created tk on success or NULL if a syntax error
 *	(unclosed quote) occurs
 */

t_tk	*create_double_quote_tk(t_ms *ms, const char *input, int *i)
{
	int		start;
	char	*raw_content;
	char	*expanded;
	t_tk	*tk;

	(*i)++;
	start = *i;
	while (input[*i] != '"' && input[*i] != '\0')
		(*i)++;
	if (input[*i] == '\0')
	{
		syn_err(ms, "unclosed double quote");
		return (NULL);
	}
	raw_content = ft_substr(input, start, *i - start);
	if (!raw_content)
		error(ms, "Error ft_substr() failed in create_double_quote_tk");
	(*i)++;
	expanded = expand_in_double_quote(ms, raw_content);
	free(raw_content);
	tk = create_tk(ms, TK_WORD, expanded);
	free(expanded);
	return (tk);
}

/* create_single_quote_tk()
 *	- skips the opening single quote by advancing the index (*i)
 *	- identifies the starting point of the content within the sinlge
 *	quotes in start
 *	- handles unclosed single quotes by reporting a syntax error and
 *	returning NULL
 *	- extracts the content between the quotes using `ft_substr`
 *	- creates a `tk_WORD` tk with the extracted content
 *	- returns the created tk, or NULL on syntax error
 */

t_tk	*create_single_quote_tk(t_ms *ms, const char *input, int *i)
{
	int		start;
	char	*content;
	t_tk	*tk;

	(*i)++;
	start = *i;
	while (input[*i] != '\'' && input[*i] != '\0')
		(*i)++;
	if (input[*i] == '\0')
	{
		syn_err(ms, "unclosed single quote");
		return (NULL);
	}
	content = ft_substr(input, start, *i - start);
	if (!content)
		error(ms, "Error ft_substr() failed in create_single_quote_tk");
	(*i)++;
	tk = create_tk(ms, TK_WORD, content);
	free(content);
	return (tk);
}

/* create_heredoc_tk
*	- creates a tk for the '<<' (heredoc) operator
*	- ensures that '<<' is not the first element of the cmd line
*	- if valid, advances the input index past the '<<' and creates the tk
*	- returns a tk if valid or NULL if not valid or if '<<' is the first
*	element, as it would be a syntax error
*/
t_tk	*create_heredoc_tk(t_ms *ms, const char *input, int *i)
{
	(void)input;
	if (*i == 0)
	{
		syn_err(ms, "'<<' can not be the first element of the cmd line");
		return (NULL);
	}
	*i += 2;
	return (create_tk(ms, TK_HEREDOC, "<<"));
}

/**
 * create_redirection_tk()
 *	- identifies and creates tks for the redirection operators:
 *		- `<<` for HEREDOC
 *		- `>>` for append output redirection tk_APPEND_OUTPUT
 *		- `>` for standard output redirection tk_REDIRECT_OUTPUT
 *		- `<` for standard input redirection tk_REDIRECT_INPUT
 *	- advances the position pointer `i` as it processes the operators
 *	- returns the tk or NULL in case of syntax error, should never
 *	the last return statement
 */
t_tk	*create_redirection_tk(t_ms *ms, const char *input, int *i)
{
	if (input[*i] == '<' && input[*i + 1] == '<')
		return (create_heredoc_tk(ms, input, i));
	else if (input[*i] == '>')
	{
		if (input[*i + 1] == '>')
		{
			*i += 2;
			return (create_tk(ms, TK_APPEND_OUTPUT, ">>"));
		}
		else
		{
			(*i)++;
			return (create_tk(ms, TK_REDIRECT_OUTPUT, ">"));
		}
	}
	else if (input[*i] == '<')
	{
		(*i)++;
		return (create_tk(ms, TK_REDIRECT_INPUT, "<"));
	}
	return (NULL);
}

/* create_pipe_tk()
 *	- if the current position is the start of the input (*i == 0), it returns
 *	NULL and displays a syntax error because '|' cannot be the first element
 *	of the input
 *	- advances the position in the input by incrementing (*i) to pass the '|'
 *	- returns a tk of type tk_PIPE with the value "|" or NULL in case of
 *	syntax error
 */
t_tk	*create_pipe_tk(t_ms *ms, const char *input, int *i)
{
	(void)input;
	if (*i == 0)
	{
		syn_err(ms, "'|' can not be the first element of the cmd line");
		return (NULL);
	}
	(*i)++;
	return (create_tk(ms, TK_PIPE, "|"));
}
