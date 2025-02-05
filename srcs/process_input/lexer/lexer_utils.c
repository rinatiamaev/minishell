/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nlouis <nlouis@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/31 20:21:20 by nlouis            #+#    #+#             */
/*   Updated: 2025/02/05 12:06:56 by nlouis           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_is_operator(char *input, int i)
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

void	detect_quoted_delimiter(t_tk *tk)
{
	int		len;
	char	*cleaned;

	if (!tk->value)
		return ;
	len = ft_strlen(tk->value);
	if (len < 2)
		return ;
	if ((tk->value[0] == '\'' && tk->value[len - 1] == '\'')
		|| (tk->value[0] == '"' && tk->value[len - 1] == '"'))
	{
		tk->delimiter_quoted = true;
		cleaned = ft_substr(tk->value, 1, len - 2);
		if (cleaned)
		{
			free(tk->value);
			tk->value = cleaned;
		}
	}
}
