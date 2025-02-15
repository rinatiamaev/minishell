/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validate_cmds.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nlouis <nlouis@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/13 17:52:04 by nlouis            #+#    #+#             */
/*   Updated: 2025/01/14 13:18:53 by nlouis           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
** assign_cmd_type - Set the type of each command in the pipeline.
**
** This function iterates through the command list linked via the pipe_to
** pointer. For each command with a name, it determines whether the
** command is a builtin or an external command. If the command is
** builtin, the builtin flag is set to true; otherwise, it is set to false.
*/
void	assign_cmd_type(t_ms *ms, t_cmd *cmd)
{
	t_cmd	*current;

	current = cmd;
	while (current)
	{
		if (current->name)
		{
			if (is_builtin(current))
				current->builtin = true;
			else if (is_external(ms, current, current->name))
				current->builtin = false;
		}
		current = current->pipe_to;
	}
}
