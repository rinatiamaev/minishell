/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nlouis <nlouis@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/02 18:32:29 by nlouis            #+#    #+#             */
/*   Updated: 2025/02/07 10:03:06 by nlouis           ###   ########.fr       */
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
	if (!tks[*i])
	{
		free_cmd(cmd);
		syn_err(ms, "near unexpected token `|'");
		return (NULL);
	}
	cmd->pipe_to = parse_tks(ms, &tks[*i]);
	if (!cmd->pipe_to)
	{
		free_cmd(cmd);
		return (NULL);
	}
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
	cmd->has_command = false;
	return (cmd);
}

static int	handle_single_token(t_ms *ms, t_cmd *cmd, t_tk **tks, int *i)
{
	if (tks[*i]->type == TK_WORD)
	{
		parse_word(ms, cmd, tks[*i]);
		cmd->has_command = true;
	}
	else if (tks[*i]->type == TK_PIPE)
	{
		if (!cmd->has_command)
		{
			free_cmd(cmd);
			syn_err(ms, "near unexpected token `|'");
			return (-1);
		}
		if (parse_pipe(ms, cmd, tks, i) == NULL)
			return (-1);
		return (1);
	}
	else
	{
		if (parse_redirections(ms, cmd, tks, i) == -1)
			return (-1);
	}
	return (0);
}

t_cmd	*parse_tks(t_ms *ms, t_tk **tks)
{
	t_cmd	*cmd;
	int		i;
	int		ret;

	i = 0;
	cmd = initialize_cmd(ms);
	while (tks[i])
	{
		ret = handle_single_token(ms, cmd, tks, &i);
		if (ret == -1)
			return (NULL);
		if (ret == 1)
			break ;
		i++;
	}
	return (cmd);
}
