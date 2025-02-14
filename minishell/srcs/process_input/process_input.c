/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_input.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nlouis <nlouis@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/28 08:48:28 by nlouis            #+#    #+#             */
/*   Updated: 2025/02/13 10:14:53 by nlouis           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	exit_program_ctrld(t_ms *ms)
{
	if (ms)
		free_all(ms);
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

int	process_input(t_ms *ms)
{
	if (get_input(ms))
		return (-1);
	if (lexer(ms))
		return (-1);
	if (expand_tks(ms))
		return (-1);
	ms->cmd = parse_tks(ms, ms->tks);
	if (!ms->cmd)
		return (-1);
	assign_cmd_type(ms, ms->cmd);
	return (0);
}
