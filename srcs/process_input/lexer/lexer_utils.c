/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nlouis <nlouis@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/31 20:21:20 by nlouis            #+#    #+#             */
/*   Updated: 2025/02/01 18:56:14 by nlouis           ###   ########.fr       */
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

t_tk	**initialize_tks(t_ms *ms)
{
	t_tk	**tks;

	tks = malloc(sizeof(t_tk *) * MAX_TKS);
	if (!tks)
		error(ms, "Error malloc failed in initialize_tks()");
	return (tks);
}
