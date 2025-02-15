/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_env.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nlouis <nlouis@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/26 13:40:48 by riamaev           #+#    #+#             */
/*   Updated: 2025/02/15 12:58:17 by nlouis           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
 * builtin_env:
 *   Prints all environment variables to standard output.
 *   If the environment is empty, an error message is printed and
 *   the exit status is set to 1. Otherwise, each variable is printed
 *   on a new line and the exit status is set to 0.
 */
void	builtin_env(t_ms *ms)
{
	int		i;

	i = 0;
	if (!ms->envp || !ms->envp[0])
	{
		builtin_err(ms, "environment is empty");
		ms->exit_status = 1;
		return ;
	}
	while (ms->envp[i])
	{
		printf("%s\n", ms->envp[i]);
		i++;
	}
	ms->exit_status = 0;
}
