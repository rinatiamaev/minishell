/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errors.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nlouis <nlouis@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/28 08:54:46 by nlouis            #+#    #+#             */
/*   Updated: 2025/02/05 14:32:32 by nlouis           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	syn_err(t_ms *ms, char *error_message)
{
	ms->exit_status = 2;
	ft_putstr_fd(RED "⭕ Syntax error: " RESET, STDERR_FILENO);
	ft_putstr_fd(error_message, STDERR_FILENO);
	ft_putstr_fd("\n", STDERR_FILENO);
}

void	cmd_err(t_cmd *cmd, char *error_message)
{
	ft_putstr_fd(RED "⭕ Error: " RESET, STDERR_FILENO);
	ft_putstr_fd(cmd->name, STDERR_FILENO);
	ft_putstr_fd(": ", STDERR_FILENO);
	ft_putstr_fd(error_message, STDERR_FILENO);
	ft_putstr_fd("\n", STDERR_FILENO);
}

void	error(t_ms *ms, char *error_message)
{
	ft_putstr_fd(RED "❌ Error: " RESET, STDERR_FILENO);
	if (ms->cmd->builtin)
	{
		ft_putstr_fd(ms->cmd->name, STDERR_FILENO);
		ft_putstr_fd(": ", STDERR_FILENO);
	}
	ft_putstr_fd(error_message, STDERR_FILENO);
	ft_putstr_fd("\n", STDERR_FILENO);
	if (errno)
		perror(error_message);
	if (ms)
		free_all(ms);
	rl_clear_history();
	exit(EXIT_FAILURE);
}

void	builtin_err(t_ms *ms, char *error_message)
{
	ft_putstr_fd(RED "⭕ Error: " RESET, STDERR_FILENO);
	ft_putstr_fd(ms->cmd->name, STDERR_FILENO);
	ft_putstr_fd(": ", STDERR_FILENO);
	ft_putstr_fd(error_message, STDERR_FILENO);
	ft_putstr_fd("\n", STDERR_FILENO);
}
