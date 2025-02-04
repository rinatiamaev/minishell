/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nlouis <nlouis@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/31 20:21:20 by nlouis            #+#    #+#             */
/*   Updated: 2025/02/04 14:34:41 by nlouis           ###   ########.fr       */
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

t_tk	**initialize_tks(t_ms *ms)
{
	t_tk	**tks;

	tks = ft_calloc(MAX_TKS, sizeof(t_tk *));
	if (!tks)
		error(ms, "malloc() failed in initialize_tks()");
	return (tks);
}

char	*ft_strjoin_free(char *s1, char *s2)
{
	char	*joined;

	if (!s1 && !s2)
		return (NULL);
	if (!s1)
		return (s2);
	if (!s2)
		return (s1);
	joined = ft_strjoin(s1, s2);
	if (!joined)
	{
		free(s1);
		free(s2);
		return (NULL);
	}
	free(s1);
	free(s2);
	return (joined);
}
