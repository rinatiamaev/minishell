/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_input_redir.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nlouis <nlouis@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/13 12:57:00 by nlouis            #+#    #+#             */
/*   Updated: 2025/02/14 14:28:28 by nlouis           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static t_redir	*init_input_redir(t_ms *ms, int *i)
{
	t_redir	*rd;

	rd = x_malloc(ms, sizeof(t_redir));
	rd->is_input = true;
	rd->is_heredoc = false;
	rd->is_append = false;
	rd->filename = NULL;
	rd->delimiter = NULL;
	rd->delimiter_quoted = false;
	rd->heredoc_fd = -1;
	(*i)++;
	return (rd);
}

static int	consume_input_redir_tar(t_ms *ms, t_redir *rd, t_tk **tks, int *i)
{
	if (!tks[*i])
		return (syn_err(ms, "expected file/delim after redirection op"), -1);
	if (tks[*i - 1]->type == TK_HEREDOC)
	{
		if (tks[*i]->type != TK_HEREDOC_DELIMITER)
		{
			syn_err(ms, "expected heredoc delimiter token after '<<'");
			return (-1);
		}
		rd->delimiter = x_strdup(ms, tks[*i]->value);
		rd->delimiter_quoted = tks[*i]->delimiter_quoted;
	}
	else
	{
		if (tks[*i]->type != TK_WORD)
		{
			syn_err(ms, "invalid token after '<'");
			return (-1);
		}
		rd->filename = x_strdup(ms, tks[*i]->value);
	}
	(*i)++;
	return (0);
}

static t_redir	*create_input_redir(t_ms *ms, t_tk **tks, int *i)
{
	t_redir	*rd;

	rd = init_input_redir(ms, i);
	if (!rd)
		return (NULL);
	if (consume_input_redir_tar(ms, rd, tks, i) == -1)
	{
		free(rd);
		return (NULL);
	}
	if (tks[*i - 2]->type == TK_HEREDOC)
		rd->is_heredoc = true;
	return (rd);
}

static int	append_input_redir_to_cmd(t_cmd *cmd, t_redir *rd)
{
	int		count;
	t_redir	**new_arr;

	count = 0;
	while (cmd->input_redirs && cmd->input_redirs[count])
		count++;
	new_arr = ft_realloc(cmd->input_redirs, sizeof(t_redir *) * count,
			sizeof(t_redir *) * (count + 2));
	if (!new_arr)
		return (-1);
	cmd->input_redirs = new_arr;
	cmd->input_redirs[count] = rd;
	cmd->input_redirs[count + 1] = NULL;
	return (0);
}

int	append_input_redir(t_ms *ms, t_cmd *cmd, t_tk **tks, int *i)
{
	t_redir	*rd;

	rd = create_input_redir(ms, tks, i);
	if (!rd)
		return (-1);
	if (append_input_redir_to_cmd(cmd, rd) == -1)
		error(ms, "append_input_redir(): ft_realloc failed");
	return (0);
}
