/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cmds.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: riamaev <riamaev@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/26 20:15:57 by riamaev           #+#    #+#             */
/*   Updated: 2025/01/26 20:15:59 by riamaev          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	execute_external_cmd(t_ms *ms, t_cmd *cmd)
{
	int		fd[2];
	int		prev_fd;
	int		next_fd;
	pid_t	pid;

	prev_fd = -1;
	while (cmd)
	{
		next_fd = -1;
		handle_pipe(ms, cmd, fd, &next_fd);
		pid = fork();
		if (pid == -1)
			error(ms, "fork() failed");
		if (pid == 0)
			child_process(ms, prev_fd, next_fd, cmd);
		else
			parent_process(ms, pid, fd, &prev_fd);
		cmd = cmd->pipe_to;
	}
}

void	execute_cmd(t_ms *ms, t_cmd *cmd)
{
	if (cmd->builtin)
		execute_builtin_cmd(ms, cmd);
	else if (cmd->path)
		execute_external_cmd(ms, cmd);
}

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
	else
	{
		write(2, "Unknown builtin command: ", 25);
		write(2, cmd->name, ft_strlen(cmd->name));
		write(2, "\n", 1);
	}
}
