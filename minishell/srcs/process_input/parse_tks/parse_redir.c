/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_redirion.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nlouis <nlouis@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/12 10:16:12 by nlouis            #+#    #+#             */
/*   Updated: 2025/02/13 10:16:19 by nlouis           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
 * parse_redir:
 *   Parses redirection tokens from the token list and updates
 *   the command structure accordingly. Depending on the token
 *   type, it calls the appropriate function to append an input or
 *   output redirection to the command.
 */
int	parse_redir(t_ms *ms, t_cmd *cmd, t_tk **tks, int *i)
{
	t_tk_type	type;

	type = tks[*i]->type;
	if (type == TK_REDIRECT_INPUT || type == TK_HEREDOC)
		return (append_input_redir(ms, cmd, tks, i));
	else if (type == TK_REDIRECT_OUTPUT || type == TK_APPEND_OUTPUT)
		return (append_output_redir(ms, cmd, tks, i));
	return (0);
}
