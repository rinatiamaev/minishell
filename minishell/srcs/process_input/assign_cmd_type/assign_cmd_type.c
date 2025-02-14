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
