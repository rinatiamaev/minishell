/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   child_process.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nlouis <nlouis@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/26 19:00:13 by riamaev           #+#    #+#             */
/*   Updated: 2025/01/30 13:01:00 by nlouis           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* read_heredoc_into_pipe():
 *  Reads lines from user until `delimiter` is reached (or user ^D).
 *  Writes each line into the pipe's write end (heredoc_pipe[1]).
 */
static void	read_heredoc_into_pipe(int write_fd, const char *delimiter)
{
	char	*line;

	while (true)
	{
		line = readline(BOLD_BLUE "heredoc🔹> " RESET);
		if (!line || ft_strcmp(line, delimiter) == 0)
			break ;
		write(write_fd, line, ft_strlen(line));
		write(write_fd, "\n", 1);
		free(line);
	}
	free(line);
	close(write_fd);
}

/* setup_heredoc_in_child():
 *  Creates a pipe just for heredoc data, reads lines into the write end,
 *  then dup2(...) the read end onto STDIN.
 */
int	setup_heredoc_in_child(t_cmd *cmd)
{
	int	heredoc_pipe[2];

	if (!cmd->heredoc_delimiter)
		return (0);
	if (pipe(heredoc_pipe) == -1)
	{
		perror("pipe() failed for heredoc");
		return (-1);
	}
	read_heredoc_into_pipe(heredoc_pipe[1], cmd->heredoc_delimiter);
	if (dup2(heredoc_pipe[0], STDIN_FILENO) == -1)
	{
		perror("dup2() failed for heredoc read end");
		close(heredoc_pipe[0]);
		return (-1);
	}
	close(heredoc_pipe[0]);
	return (0);
}

/* setup_argv:
 *  Prepares argv array for execve by combining cmd->name + cmd->args.
 */
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

/* setup_pipe_redirection():
 *  If prev_fd != -1, dup2(...) it to STDIN
 *  If next_fd != -1, dup2(...) it to STDOUT
 */
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

/* child_process:
 *  1) If we have a heredoc, set it up (pipe + read lines).
 *  2) Setup pipeline bridging (prev_fd->stdin, stdout->next_fd).
 *  3) Setup other redirections (<, >, >>).
 *  4) If builtin, run in child, else execve.
 */
void	child_process(t_ms *ms, int prev_fd, int next_fd, t_cmd *cmd)
{
	char	**argv;

	if (setup_heredoc_in_child(cmd) == -1)
		exit(ms->exit_status = 1);
	if (setup_pipe_redirection(prev_fd, next_fd) == -1)
		exit(ms->exit_status = 1);
	if (setup_redirections(cmd) == -1)
		exit(ms->exit_status = 1);
	if (cmd->builtin)
	{
		execute_builtin_cmd(ms, cmd);
		exit(ms->exit_status);
	}
	argv = setup_argv(ms, cmd);
	if (execve(cmd->path, argv, ms->envp) == -1)
	{
		error(ms, "execve() failed");
		exit(ms->exit_status = 1);
	}
	free(argv);
	exit(EXIT_FAILURE);
}
