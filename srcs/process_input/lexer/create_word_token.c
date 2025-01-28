/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_word_token.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nlouis <nlouis@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/28 09:35:09 by nlouis            #+#    #+#             */
/*   Updated: 2025/01/28 09:39:54 by nlouis           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* create_word_tk()
 *	- handles literals and words that are not enclosed by quotes or are
 *	special operators (like `|`, `<`, `>`).
 *	- skips over any unsupported characters (`;` and `\`), reporting a
 *	syntax error if encountered
 	- identifies the start of the word and iterates until encountering:
 *    - whitespace
 *    - Special characters (`|`, `<`, `>`)
 *    - End of input (`\0`)
 *	- extracts the word using `ft_substr`
 *	- expands environment variables in the word using `expand_env_in_word`
 *	- returns a `tk_WORD` with the expanded content, NULL if an unsupported
 *	character is encountered (`;` or `\`)
 */
t_tk	*create_word_tk(t_ms *ms, const char *input, int *i)
{
	int		start;
	char	*raw_content;
	char	*expanded;
	t_tk	*tk;

	start = *i;
	while (!ft_isspace(input[*i]) && input[*i] != '|'
		&& input[*i] != '<' && input[*i] != '>' && input[*i] != '\0')
	{
		if (input[*i] == ';' || input[*i] == '\\')
		{
			syn_err(ms, "characters ';' and '\\' are not supported");
			return (NULL);
		}
		(*i)++;
	}
	raw_content = ft_substr(input, start, *i - start);
	if (!raw_content)
		error(ms, "Error ft_substr() failed in create_word_tk");
	expanded = expand_env_in_word(ms, raw_content);
	free(raw_content);
	tk = create_tk(ms, TK_WORD, expanded);
	free(expanded);
	return (tk);
}
