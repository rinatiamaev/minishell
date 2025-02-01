/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nlouis <nlouis@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/31 20:21:20 by nlouis            #+#    #+#             */
/*   Updated: 2025/02/01 09:23:30 by nlouis           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_is_operator(const char *input, int i)
{
	if (input[i] == '|')
		return (1);
	if (input[i] == '<')
	{
		if (input[i + 1] == '<')
			return (2);
		return (1);
	}
	if (input[i] == '>')
	{
		if (input[i + 1] == '>')
			return (2);
		return (1);
	}
	return (0);
}