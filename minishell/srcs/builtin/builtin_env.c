/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_env.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nlouis <nlouis@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/26 13:40:48 by riamaev           #+#    #+#             */
/*   Updated: 2025/01/31 10:49:11 by nlouis           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
