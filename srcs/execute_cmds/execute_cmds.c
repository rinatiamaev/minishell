/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_cmds.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nlouis <nlouis@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/29 16:25:47 by nlouis            #+#    #+#             */
/*   Updated: 2025/01/29 16:25:48 by nlouis           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	parent_process(t_ms *ms, pid_t pid, int *fd, int *prev_fd)
{
	int	status;

	if (fd[1] != -1)
		close(fd[1]);
	if (*prev_fd != -1)
		close(*prev_fd);
	*prev_fd = fd[0];
	waitpid(pid, &status, 0);
	if (WIFEXITED(status))
		ms->exit_status = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
		ms->exit_status = 128 + WTERMSIG(status);
}

static void	handle_pipe(t_ms *ms, t_cmd *cmd, int *fd, int *next_fd)
{
	if (cmd->pipe_to)
	{
		if (pipe(fd) == -1)
			error(ms, "pipe() failed");
		*next_fd = fd[1];
	}
	else
	{
		fd[0] = -1;
		fd[1] = -1;
	}
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

void	execute_cmd(t_ms *ms, t_cmd *cmd)
{
	int		fd[2];
	int		prev_fd;
	int		next_fd;
	pid_t	pid;

	prev_fd = -1;
	if (!cmd->pipe_to && cmd->builtin)
		execute_builtin_cmd(ms, cmd);
	else
	{
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
}
