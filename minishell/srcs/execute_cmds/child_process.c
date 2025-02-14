/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   child_process.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nlouis <nlouis@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/26 19:00:13 by riamaev           #+#    #+#             */
/*   Updated: 2025/02/14 13:39:35 by nlouis           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	**setup_argv(t_cmd *cmd)
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
		cmd_err(cmd, "malloc failed in setup_argv");
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

static void	setup_input(t_ms *ms, int prev_fd, t_cmd *cmd)
{
	bool	has_input_redir;

	has_input_redir = (cmd->input_redirs && cmd->input_redirs[0]);
	if (has_input_redir)
	{
		if (prev_fd != -1)
			close(prev_fd);
	}
	else
	{
		if (prev_fd != -1)
		{
			if (dup2(prev_fd, STDIN_FILENO) == -1)
			{
				cmd_err(cmd, "dup2(next_fd) failed");
				exit(ms->exit_status = 1);
			}
			close(prev_fd);
		}
	}
}

static void	setup_output(t_ms *ms, int next_fd, t_cmd *cmd)
{
	bool	has_output_redir;

	has_output_redir = (cmd->output_redirs && cmd->output_redirs[0]);
	if (has_output_redir)
	{
		if (next_fd != -1)
			close(next_fd);
	}
	else
	{
		if (next_fd != -1)
		{
			if (dup2(next_fd, STDOUT_FILENO) == -1)
			{
				cmd_err(cmd, "dup2(next_fd) failed");
				exit(ms->exit_status = 1);
			}
			close(next_fd);
		}
	}
}

static void	execute_child(t_ms *ms, t_cmd *cmd)
{
	char		**argv;
	struct stat	st;

	if (stat(cmd->path, &st) == 0 && S_ISDIR(st.st_mode))
	{
		cmd_err(cmd, "Is a directory");
		exit(ms->exit_status = 126);
	}
	argv = setup_argv(cmd);
	if (!argv)
		exit(ms->exit_status = 1);
	execve(cmd->path, argv, ms->envp);
	if (errno)
	{
		cmd_err(cmd, NULL);
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
	if (!cmd->name)
		exit(ms->exit_status = 0);
	setup_input(ms, prev_fd, cmd);
	setup_output(ms, next_fd, cmd);
	if (setup_redir(ms, cmd) == -1)
		exit(ms->exit_status = 1);
	if (cmd->builtin)
	{
		execute_builtin_cmd(ms, cmd);
		exit(ms->exit_status);
	}
	execute_child(ms, cmd);
}
