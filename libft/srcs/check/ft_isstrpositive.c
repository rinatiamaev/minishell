/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_isstrpositive.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nlouis <nlouis@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/08 14:09:44 by nlouis            #+#    #+#             */
/*   Updated: 2024/11/08 14:15:35 by nlouis           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_isstrpositive(char *str)
{
	int	value;

	if (str == NULL || str[0] == '\0')
		return (0);
	value = ft_atoi(str);
	if (value < 0)
		return (0);
	return (1);
}
