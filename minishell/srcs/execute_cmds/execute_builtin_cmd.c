/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_builtin_cmd.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nlouis <nlouis@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/29 16:25:47 by nlouis            #+#    #+#             */
/*   Updated: 2025/02/15 13:26:24 by nlouis           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
 * execute_builtin_cmd:
 *   Determines which builtin command to execute based on the command name.
 *   It compares cmd->name with known builtin commands and calls the
 *   corresponding function with the minishell context (ms) and command (cmd).
 *   Supported builtins: echo, cd, pwd, export, unset, env, and exit.
 */
void	execute_builtin_cmd(t_ms *ms, t_cmd *cmd)
{
	if (ft_strcmp(cmd->name, "echo") == 0)
		builtin_echo(ms, cmd);
	else if (ft_strcmp(cmd->name, "cd") == 0)
		builtin_cd(ms, cmd);
	else if (ft_strcmp(cmd->name, "pwd") == 0)
		builtin_pwd(ms);
	else if (ft_strcmp(cmd->name, "export") == 0)
		builtin_export(ms, cmd);
	else if (ft_strcmp(cmd->name, "unset") == 0)
		builtin_unset(ms, cmd);
	else if (ft_strcmp(cmd->name, "env") == 0)
		builtin_env(ms);
	else if (ft_strcmp(cmd->name, "exit") == 0)
		builtin_exit(ms, cmd);
}
