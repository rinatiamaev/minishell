/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nlouis <nlouis@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/20 21:00:10 by nlouis            #+#    #+#             */
/*   Updated: 2024/12/11 18:39:25 by nlouis           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	ft_check_overflow(long long num, const char *str, int sign)
{
	if (num > (INT_MAX - (*str - '0')) / 10)
	{
		if (sign == 1)
			return (-1);
		else
			return (0);
	}
	return (1);
}

int	ft_atoi(const char *str)
{
	long long	num;
	int			sign;
	int			overflow_status;

	num = 0;
	sign = 1;
	while (ft_isspace(*str))
		str++;
	if (*str == '+' || *str == '-')
	{
		if (*str == '-')
			sign = -1;
		str++;
	}
	while (*str && ft_isdigit(*str))
	{
		overflow_status = ft_check_overflow(num, str, sign);
		if (overflow_status != 1)
			return (overflow_status);
		num = num * 10 + *str - '0';
		str++;
	}
	return ((int)num * sign);
}
