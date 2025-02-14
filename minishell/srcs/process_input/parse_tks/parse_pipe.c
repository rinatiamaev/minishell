/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_pipe.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nlouis <nlouis@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/13 10:29:55 by nlouis            #+#    #+#             */
/*   Updated: 2025/02/13 12:57:50 by nlouis           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_cmd	*parse_pipe(t_ms *ms, t_tk **tks, int *i)
{
	(*i)++;
	if (!tks[*i])
	{
		syn_err(ms, "syntax error near unexpected token `|'");
		return (NULL);
	}
	return (new_cmd(ms, NULL));
}
