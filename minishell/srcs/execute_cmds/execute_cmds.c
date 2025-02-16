/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_cmds.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nlouis <nlouis@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/29 16:25:47 by nlouis            #+#    #+#             */
/*   Updated: 2025/02/16 17:52:46 by nlouis           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
 * handle_pipe:
 *   Sets up a pipe for the current command if there is a piped command.
 *   If cmd->pipe_to exists, a pipe is created, the close-on-exec flag is
 *   set on both ends, and the write end is stored in next_fd. Otherwise,
 *   both file descriptors are set to -1.
 */
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

/*
 * execute_builtin_without_pipe:
 *   Executes a builtin command when no pipe is present. It saves the
 *   current STDIN and STDOUT, sets up redirections, runs the builtin, and
 *   restores the original file descriptors.
 */
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

/*
 * wait_for_children:
 *   Waits for all child processes spawned to execute the pipeline.
 *   It loops using waitpid() and, when the last child's PID is reaped,
 *   updates ms->exit_status with its exit status (or signal value).
 */
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

/*
 * execute_single_cmd:
 *   Forks a child process to execute a single command in the pipeline.
 *   It first sets up a pipe (if needed) via handle_pipe, then forks.
 *   In the child, child_process() is called to execute the command.
 *   The parent closes the appropriate file descriptors and updates prev_fd.
 */
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

/*
 * execute_cmd:
 *   Executes a chain of commands, possibly connected by pipes.
 *   It first reads all heredocs. For a single builtin without a pipe,
 *   it calls execute_builtin_without_pipe(). Otherwise, it forks a child
 *   process for each command in the pipeline using execute_single_cmd() and
 *   then waits for all child processes to finish.
 */
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
