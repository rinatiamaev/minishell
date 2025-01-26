/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_idux.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nlouis <nlouis@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/29 11:54:20 by nlouis            #+#    #+#             */
/*   Updated: 2024/12/02 12:05:57 by nlouis           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	format_and_print_id(t_fmt *fmt, va_list *args, t_pf *pf)
{
	int				num;
	char			*str;
	int				len;

	num = va_arg(*args, int);
	str = ft_itoa(num);
	if (str == NULL)
	{
		pf->malloc_error = 1;
		return ;
	}
	len = ft_strlen(str);
	str = handle_precision_int(fmt, str, &len, pf);
	if (str == NULL)
		return ;
	print_formatted_number(fmt, str, len, pf);
	if (pf->write_error != 0)
		return ;
}

void	format_and_print_u(t_fmt *fmt, va_list *args, t_pf *pf)
{
	unsigned int	num;
	char			*str;
	int				len;

	num = va_arg(*args, unsigned int);
	str = ft_utoa(num);
	if (str == NULL)
		return ;
	len = ft_strlen(str);
	str = handle_precision_int(fmt, str, &len, pf);
	if (str == NULL)
		return ;
	print_formatted_number(fmt, str, len, pf);
	if (pf->write_error != 0)
		return ;
}

void	format_and_print_x(t_fmt *fmt, va_list *args, int uppercase, t_pf *pf)
{
	unsigned int	num;
	char			*str;
	int				len;

	num = va_arg(*args, unsigned int);
	str = ft_itoa_base(num, 16, uppercase);
	if (str == NULL)
		return ;
	len = ft_strlen(str);
	str = handle_precision_int(fmt, str, &len, pf);
	if (str == NULL)
		return ;
	print_formatted_number(fmt, str, len, pf);
	if (pf->write_error != 0)
		return ;
}
