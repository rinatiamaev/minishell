/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_tokens_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nlouis <nlouis@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/28 09:28:16 by nlouis            #+#    #+#             */
/*   Updated: 2025/01/28 09:40:03 by nlouis           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* create_tk()
 *	- allocates and initializes a tk with the given type and value.
 *	- duplicates the value string if provided; otherwise, sets it to NULL.
 *	- handles memory allocation errors via the error handler.
 *	- returns: a pointer to the created tk.
 */
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
			error(ms, "ft_strdup() failed in create_tk()");
	}
	else
		tk->value = NULL;
	return (tk);
}
