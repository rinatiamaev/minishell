/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_output_redir.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nlouis <nlouis@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/13 11:06:06 by nlouis            #+#    #+#             */
/*   Updated: 2025/02/14 14:28:09 by nlouis           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static t_redir	*init_output_redir(t_ms *ms, t_tk **tks, int *i)
{
	t_redir	*rd;

	rd = x_malloc(ms, sizeof(t_redir));
	rd->is_input = false;
	rd->is_heredoc = false;
	rd->filename = NULL;
	rd->delimiter = NULL;
	rd->delimiter_quoted = false;
	if (tks[*i]->type == TK_APPEND_OUTPUT)
		rd->is_append = true;
	else
		rd->is_append = false;
	(*i)++;
	return (rd);
}

static int	consume_output_filename(t_ms *ms, t_redir *rd, t_tk **tks, int *i)
{
	if (!tks[*i])
	{
		syn_err(ms, "expected a file after '>' or '>>'");
		return (-1);
	}
	if (tks[*i]->type != TK_WORD)
	{
		syn_err(ms, "invalid token after '>' or '>>'");
		return (-1);
	}
	rd->filename = x_strdup(ms, tks[*i]->value);
	(*i)++;
	return (0);
}

static t_redir	*create_output_redir(t_ms *ms, t_tk **tks, int *i)
{
	t_redir	*rd;

	rd = init_output_redir(ms, tks, i);
	if (consume_output_filename(ms, rd, tks, i) == -1)
	{
		free(rd);
		return (NULL);
	}
	return (rd);
}

static int	append_output_redir_to_cmd(t_cmd *cmd, t_redir *rd)
{
	int		count;
	t_redir	**new_arr;

	count = 0;
	while (cmd->output_redirs && cmd->output_redirs[count])
		count++;
	new_arr = ft_realloc(cmd->output_redirs, sizeof(t_redir *) * count,
			sizeof(t_redir *) * (count + 2));
	if (!new_arr)
		return (-1);
	cmd->output_redirs = new_arr;
	cmd->output_redirs[count] = rd;
	cmd->output_redirs[count + 1] = NULL;
	return (0);
}

int	append_output_redir(t_ms *ms, t_cmd *cmd, t_tk **tks, int *i)
{
	t_redir	*rd;

	rd = create_output_redir(ms, tks, i);
	if (!rd)
		return (-1);
	if (append_output_redir_to_cmd(cmd, rd) == -1)
		error(ms, "append_output_redir(): ft_realloc failed");
	return (0);
}
