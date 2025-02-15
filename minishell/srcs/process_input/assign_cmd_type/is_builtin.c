/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   is_builtin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nlouis <nlouis@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/28 08:44:06 by nlouis            #+#    #+#             */
/*   Updated: 2025/02/15 13:25:31 by nlouis           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
** is_builtin - Check if the command is a builtin.
**
** This function compares the command name with a list of builtin
** commands. If a match is found, the command's builtin flag is set to
** true and the function returns true. Otherwise, it returns false.
*/
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
