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

bool	validate_cmds_chunks(t_ms *ms, t_cmd *cmd, char **envp)
{
	if (!cmd)
		return (true);
	if (!cmd->name && (cmd->heredoc_delimiter
			|| cmd->input_redirect || cmd->output_redirect))
		return (validate_cmds_chunks(ms, cmd->pipe_to, envp));
	if (!cmd->name)
		return (false);
	if (!is_builtin(cmd) && !is_external(ms, cmd, cmd->name))
		return (false);
	return (validate_cmds_chunks(ms, cmd->pipe_to, envp));
}
