/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: riamaev <riamaev@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/26 16:51:50 by riamaev           #+#    #+#             */
/*   Updated: 2025/01/26 17:28:52 by riamaev          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	print_env(t_ms *ms)
{
	int	i;

	i = 0;
	while (ms->envp[i])
	{
		printf("%s\n", ms->envp[i]);
		i++;
	}
	ms->exit_status = 0;
}

void	builtin_export(t_ms *ms, t_cmd *cmd)
{
	int			i;
	const char	*arg;

	i = 0;
	if (!cmd->args || !cmd->args[0])
	{
		print_env(ms);
		return ;
	}
	while (cmd->args[i])
	{
		arg = cmd->args[i];
		if (ft_strchr(arg, '='))
			process_env_var_with_equal(ms, arg);
		else if (is_valid_identifier(arg))
			process_valid_identifier(ms, arg);
		else
		{
			write(2, "minishell: export: `", 19);
			write(2, arg, ft_strlen(arg));
			write(2, "`: not a valid identifier\n", 25);
			ms->exit_status = 1;
		}
		i++;
	}
}
