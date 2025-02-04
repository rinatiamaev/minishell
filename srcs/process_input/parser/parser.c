/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nlouis <nlouis@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/02 18:32:29 by nlouis            #+#    #+#             */
/*   Updated: 2025/02/04 14:58:35 by nlouis           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* parse_pipe()
 * 	- advances the tk index (*i) past the `|` tk.
 *	- validates the presence of the next tk:
 *		- if no tk exists or the next tk is not a `tk_WORD`,
 *		reports a syntax error
 *	- calls `parse_tks` recursively to parse the tks after the pipe (`|`)
 *	creating the next cmd in the pipeline
 *	- assigns the newly parsed cmd to the `pipe_to` field of the current
 *	cmd structure
 *	- returns the updated cmd structure, or 
 *	`NULL` if a syntax error occurs or if parsing the piped cmd fails
 */
static t_cmd	*parse_pipe(t_ms *ms, t_cmd *cmd, t_tk **tks, int *i)
{
	(*i)++;
	if (!tks[*i] || tks[*i]->type != TK_WORD)
	{
		syn_err(ms, "invalid tk near '|'");
		return (NULL);
	}
	cmd->pipe_to = parse_tks(ms, &tks[*i]);
	if (!cmd->pipe_to)
		return (NULL);
	return (cmd);
}

/* initialize_cmd()
 *	- allocates memory for a `t_cmd` structure
 *	- uses `ft_memset()` to zero-initialize the structure
 *	- returns: allocated and initialized `t_cmd` structure.
 */
static t_cmd	*initialize_cmd(t_ms *ms)
{
	t_cmd	*cmd;

	cmd = malloc(sizeof(t_cmd));
	if (!cmd)
		error(ms, "Error malloc() failed in initialize_cmd");
	ft_memset(cmd, 0, sizeof(t_cmd));
	return (cmd);
}

/* parse_tks()
 *	- initializes a new `t_cmd` structure to represent the parsed cmd
 *	- iterates through the tks array to process each tk:
 *		- `tk_WORD`: Calls `parse_word()` to set the cmd or add argument
 *		- `tk_PIPE`: Indicates a pipeline; calls `parse_pipe()` to link the 
 *		current cmd to the next in the pipeline and exits the loop
 *		- redirection tks (`<`, `>`, `>>`, `<<`): Calls parse_redirections()
 *      to handle input/output redirections or heredoc delimiters
 *	- returns: ptr to the root `t_cmd` structure representing the parsed
 *	cmd, NULL in case of syntax errors or memory allocation failures.
 */
t_cmd	*parse_tks(t_ms *ms, t_tk **tks)
{
	t_cmd	*cmd;
	int		i;

	i = 0;
	cmd = initialize_cmd(ms);
	while (tks[i] != NULL)
	{
		if (tks[i]->type == TK_WORD)
			parse_word(ms, cmd, tks[i]);
		else if (tks[i]->type == TK_PIPE)
		{
			if (parse_pipe(ms, cmd, tks, &i) == NULL)
				return (NULL);
			break ;
		}
		else
		{
			if (parse_redirections(ms, cmd, tks, &i) == -1)
				return (NULL);
		}
		i++;
	}
	return (cmd);
}
