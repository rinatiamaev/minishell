/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   is_builtin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nlouis <nlouis@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/28 08:44:06 by nlouis            #+#    #+#             */
/*   Updated: 2025/02/05 22:32:35 by nlouis           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	is_builtin(t_cmd *cmd)
{
	int		i;
	char	**builtin_cmds;

	builtin_cmds = (char *[]){
		"echo",
		"cd",
		"pwd",
		"export",
		"unset",
		"env",
		"exit",
		NULL
	};
	i = 0;
	while (builtin_cmds[i])
	{
		if (ft_strcmp(cmd->name, builtin_cmds[i]) == 0)
		{
			cmd->builtin = true;
			return (true);
		}
		i++;
	}
	return (false);
}
