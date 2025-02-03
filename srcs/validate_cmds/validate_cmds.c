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

static bool	is_cmd_valid(t_ms *ms, t_cmd *cmd)
{
	if (is_builtin(cmd) == true)
	{
		cmd->builtin = true;
		return (true);
	}
	if (is_external(ms, cmd, cmd->name) == true)
	{
		cmd->builtin = false;
		return (true);
	}
	ms->exit_status = 127;
	return (false);
}

bool	validate_cmds(t_ms *ms, t_cmd *cmd, char **envp)
{
	if (!cmd)
		return (true);
	if ((!cmd->name && cmd->heredoc_delimiter)
		|| (!cmd->name && cmd->input_redirect)
		|| (!cmd->name && cmd->output_redirect))
		return (true);
	if (!is_cmd_valid(ms, cmd))
	{
		if (!cmd->builtin && cmd->path && access(cmd->path, X_OK) != 0)
		{
			cmd_err(ms, "command not executable");
			ms->exit_status = 126;
		}
		else
		{
			cmd_err(ms, "command not found");
			ms->exit_status = 127;
		}
		return (false);
	}
	return (validate_cmds(ms, cmd->pipe_to, envp));
}
