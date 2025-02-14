/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putarray.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nlouis <nlouis@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/13 22:17:44 by nlouis            #+#    #+#             */
/*   Updated: 2024/12/13 22:45:51 by nlouis           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_putintarray(int *array, int size)
{
	int	bytes_written;
	int	i;

	i = 0;
	while (i < size)
	{
		bytes_written = ft_printf("%d ", array[i]);
		i++;
	}
	return (bytes_written);
}
