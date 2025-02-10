/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_input.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nlouis <nlouis@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/28 08:48:28 by nlouis            #+#    #+#             */
/*   Updated: 2025/02/10 14:13:25 by nlouis           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	exit_program_ctrld(t_ms *ms)
{
	if (ms)
		free_all(ms);
	// ft_putstr_fd(BOLD_BLUE "👋 minishell🔹 Exit\n" RESET, STDIN_FILENO);
	rl_clear_history();
	exit(EXIT_SUCCESS);
}

static int	get_input(t_ms *ms)
{
	ms->input = readline("minishell - ");
	if (!ms->input)
		exit_program_ctrld(ms);
	if (errno)
		error(ms, "readline() failed in get_input()");
	if (*ms->input == '\0')
		return (-1);
	add_history(ms->input);
	return (0);
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
	if (get_input(ms))
		return (-1);
	if (lexer(ms))
		return (-1);
	// print_tks(ms->tks);
	ms->cmd = parse_tks(ms, ms->tks);
	if (!ms->cmd)
		return (-1);
	// print_cmd(ms->cmd, 0);
	if (!validate_cmds_chunks(ms, ms->cmd, ms->envp))
		return (-1);
	return (0);
}
