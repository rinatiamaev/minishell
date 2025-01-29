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

/*
function is_n_flag checks if an argument starts with - followed by
only n characters. builtin_echo prints arguments, 
skipping the -n flag, and conditionally adds 
newline unless the flag is present.
*/

static bool	is_n_flag(const char *arg)
{
	int	i;

	i = 1;
	if (!arg || arg[0] != '-')
		return (false);
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

	i = 1;
	no_newline = false;
	while (cmd->args[i] && is_n_flag(cmd->args[i]))
	{
		no_newline = true;
		i++;
	}
	if (!no_newline)
	{
		printf("\n");
		return ;
	}
	while (cmd->args[i])
	{
		printf("%s", cmd->args[i]);
		if (cmd->args[i + 1])
			printf(" ");
		i++;
	}
	ms->exit_status = 0;
}
