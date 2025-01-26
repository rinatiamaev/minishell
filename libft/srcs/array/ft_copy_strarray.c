/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_copy_strarray.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nlouis <nlouis@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/03 00:53:03 by nlouis            #+#    #+#             */
/*   Updated: 2025/01/03 01:06:06 by nlouis           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	**ft_copy_array(char **array)
{
	int		i;
	int		j;
	char	**copy;

	i = 0;
	while (array[i])
		i++;
	copy = malloc((i + 1) * sizeof(char *));
	if (!copy)
		return (NULL);
	j = 0;
	while (j < i)
	{
		copy[j] = ft_strdup(array[j]);
		if (!copy[j])
		{
			ft_free_array((void **)copy);
			return (NULL);
		}
		j++;
	}
	copy[i] = NULL;
	return (copy);
}
