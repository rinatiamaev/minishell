/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_exit.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nlouis <nlouis@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/26 13:46:04 by riamaev           #+#    #+#             */
/*   Updated: 2025/02/03 10:05:02 by nlouis           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	validate_exit_args(t_ms *ms, t_cmd *cmd, long long *exit_status)
{
	if (cmd->args[1])
	{
		builtin_err(ms, "too many arguments");
		ms->exit_status = 1;
		return (-1);
	}
	else if (!ft_is_valid_number(cmd->args[0]))
	{
		ms->exit_status = 255;
		builtin_err(ms, "numeric argument required");
		*exit_status = ms->exit_status;
		free_all(ms);
		exit((int)*exit_status);
	}
	return (0);
}

void	builtin_exit(t_ms *ms, t_cmd *cmd)
{
	long long	exit_status;

	if (!cmd->args || !cmd->args[0])
	{
		ft_putstr_fd(BOLD_BLUE "👋 minishell🔹 Exit\n" \
		RESET, STDOUT_FILENO);
		exit_status = ms->exit_status;
		free_all(ms);
		exit(exit_status);
	}
	if (validate_exit_args(ms, cmd, &exit_status) == -1)
		return ;
	exit_status = ft_atoll(cmd->args[0]);
	ft_putstr_fd(BOLD_BLUE "👋 minishell🔹 Exit\n" RESET, STDOUT_FILENO);
	free_all(ms);
	exit((int)(exit_status % 256));
}
