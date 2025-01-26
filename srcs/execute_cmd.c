/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_cmd.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: riamaev <riamaev@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/26 19:00:13 by riamaev           #+#    #+#             */
/*   Updated: 2025/01/26 20:01:48 by riamaev          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	setup_pipe_redirection(int prev_fd, int next_fd)
{
	if (prev_fd != -1)
	{
		if (dup2(prev_fd, STDIN_FILENO) == -1)
		{
			perror("dup2() failed for prev_fd");
			return (-1);
		}
		close(prev_fd);
	}
	if (next_fd != -1)
	{
		if (dup2(next_fd, STDOUT_FILENO) == -1)
		{
			perror("dup2() failed for next_fd");
			return (-1);
		}
		close(next_fd);
	}
	return (0);
}

char	**setup_argv(t_ms *ms, t_cmd *cmd)
{
	int		argc;
	char	**argv;
	int		i;

	argc = 0;
	while (cmd->args && cmd->args[argc])
		argc++;
	argv = malloc((argc + 2) * sizeof(char *));
	if (!argv)
	{
		write(2, "malloc() failed in setup_argv()", 31);
		ms->exit_status = 1;
	}
	argv[0] = cmd->name;
	i = 0;
	while (i < argc)
	{
		argv[i + 1] = cmd->args[i];
		i++;
	}
	argv[argc + 1] = NULL;
	return (argv);
}

void	child_process(t_ms *ms, int prev_fd, int next_fd, t_cmd *cmd)
{
	char	**argv;

	if (setup_pipe_redirection(prev_fd, next_fd) == -1)
		exit(ms->exit_status = 1);
	if (cmd->builtin)
	{
		execute_builtin_cmd(ms, cmd);
		exit(ms->exit_status);
	}
	argv = setup_argv(ms, cmd);
	if (execve(cmd->path, argv, ms->envp) == -1)
	{
		perror("execve() failed");
		ms->exit_status = 1;
		free(argv);
		exit(ms->exit_status);
	}
	free(argv);
	exit(EXIT_FAILURE);
}

void	parent_process(t_ms *ms, pid_t pid, int *fd, int *prev_fd)
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

void	handle_pipe(t_ms *ms, t_cmd *cmd, int *fd, int *next_fd)
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
