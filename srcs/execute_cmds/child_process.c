/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   child_process.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nlouis <nlouis@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/26 19:00:13 by riamaev           #+#    #+#             */
/*   Updated: 2025/02/05 23:51:20 by nlouis           ###   ########.fr       */
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
		error(ms, "malloc() failed\n");
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
	struct stat	path_stat;

	if (cmd->builtin)
	{
		execute_builtin_cmd(ms, cmd);
		free(argv);
		exit(ms->exit_status);
	}
	if (!stat(cmd->path, &path_stat) && S_ISDIR(path_stat.st_mode))
	{
		cmd_err(cmd, "Is a directory");
		free(argv);
		exit(ms->exit_status = 126);
	}
	execve(cmd->path, argv, ms->envp);
	if (errno == ENOENT)
	{
		cmd_err(cmd, "Command not found");
		free(argv);
		exit(ms->exit_status = 127);
	}
	else if (errno == EACCES || errno == ENOEXEC)
	{
		cmd_err(cmd, NULL);
		free(argv);
		exit(ms->exit_status = 126);
	}
	else if (errno)
	{
		cmd_err(cmd, NULL);
		free(argv);
		exit(ms->exit_status = 1);
	}
	else
	{
		cmd_err(cmd, "execve() failed");
		free(argv);
		exit(ms->exit_status = 1);
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
