/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   child_process.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nlouis <nlouis@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/26 19:00:13 by riamaev           #+#    #+#             */
/*   Updated: 2025/02/05 14:54:52 by nlouis           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	**setup_argv(t_ms *ms, t_cmd *cmd)
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
		write(2, "malloc() failed in setup_argv()\n", 32);
		ms->exit_status = 1;
		return (NULL);
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

static int	setup_pipe_redirection(int prev_fd, int next_fd)
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

static int	child_heredoc_if_needed(t_ms *ms, t_cmd *cmd, int prev_fd)
{
	if (prev_fd == -1 && cmd->heredoc_delimiter)
	{
		if (handle_heredoc(ms, cmd, ms->tks) == -1)
			return (-1);
	}
	return (0);
}

static void	child_exec_builtin_or_command(t_ms *ms, t_cmd *cmd, char **argv)
{
	if (cmd->builtin)
	{
		execute_builtin_cmd(ms, cmd);
		exit(ms->exit_status);
	}
	if (!cmd->path)
	{
		free(argv);
		exit(ms->exit_status = 127);
	}
	if (execve(cmd->path, argv, ms->envp) == -1)
	{
		perror("execve() failed");
		free(argv);
		exit(ms->exit_status = 127);
	}
}

void	child_process(t_ms *ms, int prev_fd, int next_fd, t_cmd *cmd)
{
	char	**argv;

	if (child_heredoc_if_needed(ms, cmd, prev_fd) == -1)
		exit(ms->exit_status = 1);
	if (setup_pipe_redirection(prev_fd, next_fd) == -1)
		exit(ms->exit_status = 1);
	if (setup_redirections(ms, cmd) == -1)
		exit(ms->exit_status = 1);
	argv = setup_argv(ms, cmd);
	if (!argv)
		exit(ms->exit_status = 1);
	child_exec_builtin_or_command(ms, cmd, argv);
	free(argv);
	exit(EXIT_FAILURE);
}
