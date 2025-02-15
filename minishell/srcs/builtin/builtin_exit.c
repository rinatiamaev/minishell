/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_exit.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nlouis <nlouis@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/26 13:46:04 by riamaev           #+#    #+#             */
/*   Updated: 2025/02/15 13:25:23 by nlouis           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
 * validate_exit_args:
 *   Validates the arguments for the exit builtin command.
 *   If more than one argument is given, it prints an error and sets
 *   the exit status to 1. If the argument is not a valid number,
 *   it prints an error, sets the exit status to 2, frees all memory,
 *   and exits with that status.
 */
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
		ms->exit_status = 2;
		builtin_err(ms, "numeric argument required");
		*exit_status = ms->exit_status;
		free_all(ms);
		exit((int)*exit_status);
	}
	return (0);
}

/*
 * builtin_exit:
 *   Implements the exit builtin command.
 *   If no argument is provided, it exits with the current exit status.
 *   Otherwise, it validates and converts the argument to a number,
 *   frees all resources, and exits with the computed status modulo 256.
 */
void	builtin_exit(t_ms *ms, t_cmd *cmd)
{
	long long	exit_status;

	if (!cmd->args || !cmd->args[0])
	{
		exit_status = ms->exit_status;
		free_all(ms);
		exit(exit_status);
	}
	if (validate_exit_args(ms, cmd, &exit_status) == -1)
		return ;
	exit_status = ft_atoll(cmd->args[0]);
	free_all(ms);
	exit((int)(exit_status % 256));
}
