/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_exit.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: riamaev <riamaev@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/26 13:46:04 by riamaev           #+#    #+#             */
/*   Updated: 2025/01/26 16:44:03 by riamaev          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	builtin_exit(t_ms *ms, t_cmd *cmd)
{
	int		exit_status;
	char	*error_msg;

	error_msg = "exit: %s: numeric argument required\n";
	if (!cmd->args || !cmd->args[1])
	{
		printf("exit\n");
		exit(ms->exit_status);
	}
	exit_status = ft_atoi(cmd->args[1]);
	if (errno == ERANGE || exit_status < 0 || exit_status > 255)
	{
		write(2, error_msg, ft_strlen(error_msg));
		ms->exit_status = 2;
		return ;
	}
	printf("exit\n");
	exit(exit_status);
}
