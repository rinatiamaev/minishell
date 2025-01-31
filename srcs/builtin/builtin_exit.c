/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_exit.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nlouis <nlouis@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/26 13:46:04 by riamaev           #+#    #+#             */
/*   Updated: 2025/01/31 08:15:55 by nlouis           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static	int	validate_exit_args(t_ms *ms, t_cmd *cmd, long long *exit_status)
{
	if (!ft_is_valid_number(cmd->args[0]))
	{
		ft_putstr_fd("⭕ exit: ", STDERR_FILENO);
		ft_putstr_fd("numeric argument required\n", STDERR_FILENO);
		return (-1);
	}
	exit_status = (long long *)ft_atoll(cmd->args[0]);
	if (exit_status < 0 || exit_status > (long long *)255)
	{
		ft_putstr_fd("exit: numeric argument between 0 and 255 required\n", 2);
		ms->exit_status = 2;
		return (-1);
	}
	if (cmd->args[1])
	{
		ft_putstr_fd("exit: too many arguments\n", STDERR_FILENO);
		ms->exit_status = 1;
		return (-1);
	}
	return (0);
}

void	builtin_exit(t_ms *ms, t_cmd *cmd)
{
	long long	exit_status;
	
	if (!cmd->args || !cmd->args[0])
	{
		ft_putstr_fd(BOLD_BLUE "👋 minishell🔹 Exit\n" RESET, STDOUT_FILENO);
		exit(ms->exit_status);
	}
	if (validate_exit_args(ms, cmd, &exit_status) == -1)
		return ;
	ft_putstr_fd(BOLD_BLUE "👋 minishell🔹 Exit\n" RESET, STDOUT_FILENO);
	exit((int)exit_status);
}
