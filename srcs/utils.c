/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nlouis <nlouis@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/03 17:56:00 by nlouis            #+#    #+#             */
/*   Updated: 2025/01/19 13:55:30 by nlouis           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	reset_prompt(t_ms *ms)
{
	free_cmd(ms->cmd);
	ms->cmd = NULL;
	free_tks(ms->tks);
	ms->tks = NULL;
	free((char *)ms->input);
	ms->input = NULL;
}

void	exit_program(t_ms *ms)
{
	if (ms)
		free_all(ms);
	ft_putstr_fd(BOLD_BLUE "👋 minishell🔹 Exit\n" RESET, STDIN_FILENO);
	rl_clear_history();
	exit(EXIT_SUCCESS);
}

void	syn_err(t_ms *ms, char *error_message)
{
	ms->exit_status = 2;
	ft_putstr_fd(RED "⭕ Syntax error: " RESET, STDERR_FILENO);
	ft_putstr_fd(error_message, STDERR_FILENO);
	ft_putstr_fd("\n", STDERR_FILENO);
}

void	cmd_err(char *error_message)
{
	ft_putstr_fd(RED "⭕ Error: " RESET, STDERR_FILENO);
	ft_putstr_fd(error_message, STDERR_FILENO);
	ft_putstr_fd("\n", STDERR_FILENO);
}

void	error(t_ms *ms, char *error_message)
{
	if (ms)
		free_all(ms);
	ft_putstr_fd(RED "❌ Error: " RESET, STDERR_FILENO);
	perror(error_message);
	rl_clear_history();
	exit(EXIT_FAILURE);
}