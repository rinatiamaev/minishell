/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   x_memory_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nlouis <nlouis@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/05 12:40:58 by nlouis            #+#    #+#             */
/*   Updated: 2025/02/15 13:09:19 by nlouis           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
 * x_strdup:
 *   Duplicates a string using ft_strdup.
 *   If the allocation fails, calls error() with a failure message.
 */
char	*x_strdup(t_ms *ms, const char *s)
{
	char	*res;

	res = ft_strdup(s);
	if (!res)
		error(ms, "x_strdup: malloc failed");
	return (res);
}

/*
 * x_substr:
 *   Creates a substring from 's' starting at index 'start'
 *   with length 'len' using ft_substr.
 *   On allocation failure, calls error().
 */
char	*x_substr(t_ms *ms, const char *s, unsigned int start, int len)
{
	char	*res;

	res = ft_substr(s, start, len);
	if (!res)
		error(ms, "x_substr: malloc failed");
	return (res);
}

/*
 * x_strjoin_free:
 *   Concatenates two strings s1 and s2 using ft_strjoin,
 *   frees both s1 and s2, and returns the joined string.
 *   If allocation fails, frees s1 and s2 and calls error().
 */
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

/*
 * x_malloc:
 *   Allocates memory of the given size.
 *   If allocation fails, calls error() with a failure message.
 */
void	*x_malloc(t_ms *ms, size_t size)
{
	void	*ptr;

	ptr = malloc(size);
	if (!ptr)
		error(ms, "x_malloc: malloc failed");
	return (ptr);
}
