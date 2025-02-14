/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_cmds.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nlouis <nlouis@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/29 16:25:47 by nlouis            #+#    #+#             */
/*   Updated: 2025/02/14 14:09:14 by nlouis           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	handle_pipe(t_ms *ms, t_cmd *cmd, int *fd, int *next_fd)
{
	if (cmd->pipe_to)
	{
		if (pipe(fd) == -1)
			error(ms, "pipe() failed");
		if (ioctl(fd[0], FIOCLEX) == -1)
			error(ms, "ioctl(FIOCLEX) failed on fd[0]");
		if (ioctl(fd[1], FIOCLEX) == -1)
			error(ms, "ioctl(FIOCLEX) failed on fd[1]");
		*next_fd = fd[1];
	}
	else
	{
		fd[0] = -1;
		fd[1] = -1;
	}
}

void	execute_builtin_without_pipe(t_ms *ms, t_cmd *cmd)
{
	int		stdin_backup;
	int		stdout_backup;

	stdin_backup = dup(STDIN_FILENO);
	stdout_backup = dup(STDOUT_FILENO);
	if (stdin_backup == -1 || stdout_backup == -1)
		error(ms, "dup() failed in execute_cmd");
	if (setup_redir(ms, cmd) == -1)
		ms->exit_status = 1;
	else
		execute_builtin_cmd(ms, cmd);
	if (dup2(stdin_backup, STDIN_FILENO) == -1
		|| dup2(stdout_backup, STDOUT_FILENO) == -1)
		error(ms, "dup2() restore failed");
	close(stdin_backup);
	close(stdout_backup);
}

static void	wait_for_children(t_ms *ms, pid_t last_pid)
{
	int		status;
	pid_t	pid;

	pid = waitpid(-1, &status, 0);
	while (pid > 0)
	{
		if (pid == last_pid)
		{
			if (WIFEXITED(status))
				ms->exit_status = WEXITSTATUS(status);
			else if (WIFSIGNALED(status))
				ms->exit_status = 128 + WTERMSIG(status);
		}
		pid = waitpid(-1, &status, 0);
	}
}

static pid_t	execute_single_cmd(t_ms *ms, t_cmd *cmd, int *prev_fd)
{
	int		fd[2];
	int		next_fd;
	pid_t	pid;

	next_fd = -1;
	handle_pipe(ms, cmd, fd, &next_fd);
	pid = fork();
	if (pid == -1)
		error(ms, "fork() failed");
	if (pid == 0)
	{
		child_process(ms, *prev_fd, next_fd, cmd);
		exit(ms->exit_status);
	}
	else
	{
		if (fd[1] != -1)
			close(fd[1]);
		if (*prev_fd != -1)
			close(*prev_fd);
		*prev_fd = fd[0];
	}
	return (pid);
}

void	execute_cmd(t_ms *ms, t_cmd *cmd)
{
	int		prev_fd;
	pid_t	last_pid;

	prev_fd = -1;
	last_pid = -1;
	if (read_all_heredocs(ms, ms->cmd) < 0)
		return ;
	if (!cmd->pipe_to && cmd->builtin)
	{
		execute_builtin_without_pipe(ms, cmd);
		return ;
	}
	while (cmd)
	{
		last_pid = execute_single_cmd(ms, cmd, &prev_fd);
		cmd = cmd->pipe_to;
	}
	if (prev_fd != -1)
		close(prev_fd);
	wait_for_children(ms, last_pid);
}
