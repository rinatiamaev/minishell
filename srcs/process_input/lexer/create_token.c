/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_token.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nlouis <nlouis@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/28 09:28:16 by nlouis            #+#    #+#             */
/*   Updated: 2025/02/05 11:50:35 by nlouis           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	create_tk(t_ms *ms, int tk_index, t_tk_type type, char *value)
{
	ms->tks[tk_index] = malloc(sizeof(t_tk));
	if (!ms->tks[tk_index])
		error(ms, "malloc() failed in create_tk()");
	ms->tks[tk_index]->type = type;
	ms->tks[tk_index]->delimiter_quoted = false;
	if (value)
	{
		ms->tks[tk_index]->value = ft_strdup(value);
		if (!ms->tks[tk_index]->value)
			error(ms, "ft_strdup() failed in create_tk()");
	}
	else
		ms->tks[tk_index]->value = NULL;
}
