/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_input.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nlouis <nlouis@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/28 08:48:28 by nlouis            #+#    #+#             */
/*   Updated: 2025/01/28 10:28:39 by nlouis           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	exit_program(t_ms *ms)
{
	if (ms)
		free_all(ms);
	ft_putstr_fd(BOLD_BLUE "👋 minishell🔹 Exit\n" RESET, STDIN_FILENO);
	rl_clear_history();
	exit(EXIT_SUCCESS);
}

static char	*get_input(t_ms *ms)
{
	char	*input;

	input = readline(BOLD_BLUE "💾 minishell🔹 " RESET);
	if (!input)
		exit_program(ms);
	if (errno)
		error(ms, "readline() failed in get_input()");
	if (*input == '\0')
		return (NULL);
	add_history(input);
	return (input);
}

/* process_input()
 *	- prompts the user for input using readline(), assigns it
 *	to 'ms->input', if prompt is empty returns (-1) so a new a prompt
 *	is generated, then adds the input in history
 *	- tkizes the input and assigns it to 'ms->tks'
 *	- parses the tks and assigns it to 'ms->cmd'
 */
int	process_input(t_ms *ms)
{
	ms->input = get_input(ms);
	if (!ms->input)
		return (-1);
	ms->tks = lexer(ms, ms->input);
	if (!ms->tks)
		return (-1);
	ms->cmd = parse_tks(ms, ms->tks);
	if (!ms->cmd)
		return (-1);
	if (!validate_cmds(ms, ms->cmd, ms->envp))
		return (-1);
	return (0);
}
