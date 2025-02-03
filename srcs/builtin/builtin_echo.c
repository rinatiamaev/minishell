/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_echo.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: riamaev <riamaev@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/26 16:22:36 by riamaev           #+#    #+#             */
/*   Updated: 2025/01/26 16:30:59 by riamaev          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static bool	is_n_flag(const char *arg)
{
	int	i;

	if (!arg || arg[0] != '-' || arg[1] == '\0')
		return (false);
	i = 1;
	while (arg[i])
	{
		if (arg[i] != 'n')
			return (false);
		i++;
	}
	return (true);
}

void	builtin_echo(t_ms *ms, t_cmd *cmd)
{
	int		i;
	bool	no_newline;

	i = 0;
	no_newline = false;
	if (!cmd->args || !cmd->args[0])
	{
		printf("\n");
		ms->exit_status = 0;
		return ;
	}
	while (cmd->args[i] && is_n_flag(cmd->args[i++]))
		no_newline = true;
	while (cmd->args[i])
	{
		printf("%s", cmd->args[i]);
		if (cmd->args[i + 1])
			printf(" ");
		i++;
	}
	if (!no_newline)
		printf("\n");
	ms->exit_status = 0;
}
