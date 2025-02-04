/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_token.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nlouis <nlouis@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/28 09:28:16 by nlouis            #+#    #+#             */
/*   Updated: 2025/02/04 10:04:33 by nlouis           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_tk	*create_tk(t_ms *ms, t_tk_type type, char *value)
{
	t_tk	*tk;

	tk = malloc(sizeof(t_tk));
	if (!tk)
		error(ms, "malloc() failed in create_tk()");
	tk->type = type;
	if (value)
	{
		tk->value = ft_strdup(value);
		if (!tk->value)
		{
			free(tk);
			error(ms, "ft_strdup() failed in create_tk()");
		}
	}
	else
		tk->value = NULL;
	return (tk);
}
