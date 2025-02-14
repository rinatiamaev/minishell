/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_tks.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nlouis <nlouis@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/13 11:45:41 by nlouis            #+#    #+#             */
/*   Updated: 2025/02/14 13:24:36 by nlouis           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
