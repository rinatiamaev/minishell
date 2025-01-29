/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   child_process.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nlouis <nlouis@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/26 19:00:13 by riamaev           #+#    #+#             */
/*   Updated: 2025/01/29 16:25:58 by nlouis           ###   ########.fr       */
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

void	child_process(t_ms *ms, int prev_fd, int next_fd, t_cmd *cmd)
{
	char	**argv;

	argv = setup_argv(ms, cmd);
	if (setup_pipe_redirection(prev_fd, next_fd) == -1)
		exit(ms->exit_status = 1);
	if (setup_redirections(cmd) == -1)
		exit(ms->exit_status = 1);
	if (cmd->builtin)
	{
		execute_builtin_cmd(ms, cmd);
		exit(ms->exit_status);
	}
	else if (execve(cmd->path, argv, ms->envp) == -1)
	{
		perror("execve() failed");
		ms->exit_status = 1;
		free(argv);
		exit(ms->exit_status);
	}
	free(argv);
	exit(EXIT_FAILURE);
}
