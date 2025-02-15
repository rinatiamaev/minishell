/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_tks.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nlouis <nlouis@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/13 11:45:41 by nlouis            #+#    #+#             */
/*   Updated: 2025/02/15 12:57:46 by nlouis           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
 * parse_next_token:
 *   Parses the next token from the token array and updates the current
 *   command structure accordingly. In particular, when a pipe token (TK_PIPE)
 *   is encountered, parse_pipe is called to create a new command node for the
 *   command following the pipe. This is necessary because the pipe operator '|'
 *   separates distinct cmds in a pipeline, so each command must be represented
 *   by its own command structure. After parse_pipe returns the new command, the
 *   current command pointer is updated to point to this new node.
 *
 *   Redirection tokens (TK_REDIRECT_INPUT, TK_REDIRECT_OUTPUT, TK_APPEND_OUTPUT
 *   TK_HEREDOC) are handled by parse_redir, while word tokens are processed by
 *   parse_word. Returns 0 on success or -1 on error.
 */
static int	parse_next_token(t_ms *ms, t_cmd **current, t_tk **tks, int *i)
{
	if (tks[*i]->type == TK_PIPE)
	{
		(*current)->pipe_to = parse_pipe(ms, tks, i);
		if (!(*current)->pipe_to)
			return (-1);
		*current = (*current)->pipe_to;
		return (0);
	}
	else if (tks[*i]->type == TK_REDIRECT_INPUT
		|| tks[*i]->type == TK_REDIRECT_OUTPUT
		|| tks[*i]->type == TK_APPEND_OUTPUT
		|| tks[*i]->type == TK_HEREDOC)
	{
		if (parse_redir(ms, *current, tks, i) == -1)
			return (-1);
		return (0);
	}
	else
	{
		if (parse_word(ms, *current, tks[*i]) == -1)
			return (-1);
		(*i)++;
		return (0);
	}
}

/*
 * parse_tks:
 *   Parses the token array and builds a command structure. Returns the
 *   head of the command list or NULL if a parsing error occurs.
 */
t_cmd	*parse_tks(t_ms *ms, t_tk **tks)
{
	t_cmd	*head;
	t_cmd	*current;
	int		i;

	head = new_cmd(ms, NULL);
	current = head;
	i = 0;
	while (tks[i])
	{
		if (parse_next_token(ms, &current, tks, &i) == -1)
		{
			free_cmd(head);
			return (NULL);
		}
	}
	return (head);
}
