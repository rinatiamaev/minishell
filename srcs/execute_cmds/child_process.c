/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   child_process.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nlouis <nlouis@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/26 19:00:13 by riamaev           #+#    #+#             */
/*   Updated: 2025/02/10 14:12:22 by nlouis           ###   ########.fr       */
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
			close(prev_fd);
			return (-1);
		}
		close(prev_fd);
	}
	if (next_fd != -1)
	{
		if (dup2(next_fd, STDOUT_FILENO) == -1)
		{
			perror("dup2() failed for next_fd");
			close(next_fd);
			return (-1);
		}
		close(next_fd);
	}
	return (0);
}

static int	child_heredoc_if_needed(t_ms *ms, t_cmd *cmd, bool is_piped)
{
	if (cmd->heredoc_delimiter)
		return (handle_heredoc(ms, cmd, ms->tks, is_piped));
	return (0);
}

static void	child_exec_builtin_or_command(t_ms *ms, t_cmd *cmd, char **argv)
{
	if (!cmd->name)
		exit(ms->exit_status);
	if (cmd->builtin)
	{
		execute_builtin_cmd(ms, cmd);
		free(argv);
		exit(ms->exit_status);
	}
	execve(cmd->path, argv, ms->envp);
	if (errno)
	{
		cmd_err(cmd, "execve() failed");
		free(argv);
		if (errno == EACCES || errno == ENOEXEC)
			exit(ms->exit_status = 126);
		else if (errno == ENOENT)
			exit(ms->exit_status = 127);
		else
			exit(ms->exit_status = 1);
	}
}

void	child_process(t_ms *ms, int prev_fd, int next_fd, t_cmd *cmd)
{
	char	**argv;
	bool	is_piped;

	is_piped = (prev_fd != -1 || cmd->pipe_to != NULL);
	if (child_heredoc_if_needed(ms, cmd, is_piped) == -1)
		exit(ms->exit_status = 1);
	if (setup_pipe_redirection(prev_fd, next_fd) == -1)
		exit(ms->exit_status = 1);
	if (setup_redirections(ms, cmd) == -1)
		exit(ms->exit_status = 1);
	argv = setup_argv(ms, cmd);
	if (!argv)
		exit(ms->exit_status = 1);
	child_exec_builtin_or_command(ms, cmd, argv);
}
