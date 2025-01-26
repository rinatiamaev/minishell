/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nlouis <nlouis@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/12 14:33:58 by nlouis            #+#    #+#             */
/*   Updated: 2024/12/22 13:42:06 by nlouis           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static size_t	ft_w_count(const char *str, char c)
{
	size_t	w_count;

	w_count = 1;
	while (*str)
	{
		if (*str == c)
			w_count++;
		str++;
	}
	return (w_count);
}

static int	ft_fill_array(char **array, const char *s, char c, size_t w_count)
{
	const char	*start;
	const char	*end;
	size_t		i;

	start = s;
	i = 0;
	while (i < w_count)
	{
		end = ft_strchr(start, c);
		if (!end)
			end = start + ft_strlen(start);
		array[i] = ft_substr(start, 0, end - start);
		if (!array[i])
		{
			ft_free_array((void **)array);
			return (0);
		}
		start = end + (*end == c);
		i++;
	}
	array[i] = NULL;
	return (1);
}

char	**ft_split(const char *s, char c)
{
	char		**array;
	size_t		w_count;

	if (!s)
		return (NULL);
	w_count = ft_w_count(s, c);
	array = (char **)ft_calloc(w_count + 1, sizeof(char *));
	if (!array)
		return (NULL);
	if (!ft_fill_array(array, s, c, w_count))
	{
		free(array);
		return (NULL);
	}
	return (array);
}
