/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_env.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: riamaev <riamaev@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/26 13:40:48 by riamaev           #+#    #+#             */
/*   Updated: 2025/01/26 16:49:36 by riamaev          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	builtin_env(t_ms *ms)
{
	int		i;
	char	*error_msg;

	i = 0;
	error_msg = "env: environment is empty\n";
	if (!ms->envp)
	{
		write(2, error_msg, strlen(error_msg));
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
