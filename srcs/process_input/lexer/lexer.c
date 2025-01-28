/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nlouis <nlouis@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/02 16:20:10 by nlouis            #+#    #+#             */
/*   Updated: 2025/01/28 10:26:47 by nlouis           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* create_next_tk()
*	- determines the type of tk at the current position in the input string
*	- delegates tk creation and the advancement of the input index to the
*	appropriate function, returns the created tk which is NULL in case
*	of syntax error
* 	tk Types and Handlers:
*		- '|' -> create_pipe_tk()
*		- '<' or '>' -> create_redirection_tk()
*		- '\'' -> create_single_quote_tk()
*		- '"' -> create_double_quote_tk()
*		- any other character -> create_word_tk()	
*/
static t_tk	*create_next_tk(t_ms *ms, const char *input, int *i)
{
	if (input[*i] == '|')
		return (create_pipe_tk(ms, input, i));
	if (input[*i] == '<' || input[*i] == '>')
		return (create_redirection_tk(ms, input, i));
	if (input[*i] == '\'')
		return (create_single_quote_tk(ms, input, i));
	if (input[*i] == '"')
		return (create_double_quote_tk(ms, input, i));
	return (create_word_tk(ms, input, i));
}

/* tkize_input()
*	Processes the input string and converts it into an array of tks.
*	- skips over whithespace between each tk and assigns the next part
*	of the input into a tk
*	- each valid tk is added to the tk array
*	- stops the process if a syntax error occurs (return(-1))
*	or at the end of the input
*	- adds a NULL guard at the end of the tk array
*/
static int	tkize_input(t_ms *ms, t_tk **tks, const char *input)
{
	int		tk_index;
	int		i;
	t_tk	*tk;

	i = 0;
	tk_index = 0;
	while (input[i] != '\0')
	{
		skip_whitespace_index(input, &i);
		tk = create_next_tk(ms, input, &i);
		if (!tk)
			return (-1);
		tks[tk_index++] = tk;
	}
	tks[tk_index] = NULL;
	return (0);
}

/* initialize_tks()
*	- allocates memory for the tk array 't_tk **tks' and 
*	returns it
*/
static t_tk	**initialize_tks(t_ms *ms)
{
	t_tk	**tks;

	tks = malloc(sizeof(t_tk *) * MAX_TKS);
	if (!tks)
		error(ms, "Error malloc failed in initialize_tks()");
	return (tks);
}

/* lexer()
*	Parameters:
*	- 't_ms *ms' is only passed here and to the lower functions in
*	case of allocation memory error so all the allocated memory can be freed
*	- 'const char *input' is the raw_input coming from readline()
*	Responsabilities
*	- initializes a tk array 't_tk **tks'
*	- pass the process of tkization to tkize_input()
*	Returns:
*	- NULL only in case of syntax error
*	- the tk array 
*/
t_tk	**lexer(t_ms *ms, const char *input)
{
	t_tk	**tks;

	tks = initialize_tks(ms);
	if (tkize_input(ms, tks, input) == -1)
		return (NULL);
	return (tks);
}
