/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   x_memory_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nlouis <nlouis@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/05 12:40:58 by nlouis            #+#    #+#             */
/*   Updated: 2025/02/05 23:44:33 by nlouis           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*x_strdup(t_ms *ms, const char *s)
{
	char	*res;

	res = ft_strdup(s);
	if (!res)
		error(ms, "x_strdup: malloc failed");
	return (res);
}

char	*x_substr(t_ms *ms, const char *s, unsigned int start, int len)
{
	char	*res;

	res = ft_substr(s, start, len);
	if (!res)
		error(ms, "x_substr: malloc failed");
	return (res);
}

char	*x_strjoin_free(t_ms *ms, char *s1, char *s2)
{
	char	*joined;

	joined = ft_strjoin(s1, s2);
	if (!joined)
	{
		free(s1);
		free(s2);
		error(ms, "x_strjoin_free: malloc failed");
	}
	free(s1);
	free(s2);
	return (joined);
}
