/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   setup_input_redir.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nlouis <nlouis@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/13 13:36:26 by nlouis            #+#    #+#             */
/*   Updated: 2025/02/15 13:20:08 by nlouis           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
 * get_input_fd:
 *   Returns the file descriptor for the input redirection.
 *   For a heredoc, returns rd->heredoc_fd; otherwise,
 *   opens rd->filename for reading. On failure, sets
 *   ms->exit_status to 1 and returns -1.
 */
static int	get_input_fd(t_ms *ms, t_redir *rd)
{
	int	fd;

	if (rd->is_heredoc)
	{
		fd = rd->heredoc_fd;
		if (fd < 0)
		{
			ms->exit_status = 1;
			return (-1);
		}
	}
	else
	{
		fd = open(rd->filename, O_RDONLY);
		if (fd == -1)
		{
			redir_err(rd, NULL);
			ms->exit_status = 1;
			return (-1);
		}
	}
	return (fd);
}

/*
 * duplicate_input_fd:
 *   Duplicates the file descriptor fd to STDIN_FILENO via dup2,
 *   closes fd, and returns 0 on success. On failure, prints an
 *   error, closes fd, sets ms->exit_status to 1, and returns -1.
 */
static int	duplicate_input_fd(t_ms *ms, int fd)
{
	if (dup2(fd, STDIN_FILENO) == -1)
	{
		perror("dup2 for input");
		close(fd);
		ms->exit_status = 1;
		return (-1);
	}
	close(fd);
	return (0);
}

/*
 * handle_one_input_redir:
 *   Handles one input redirection by obtaining the input fd via
 *   get_input_fd and duplicating it to STDIN_FILENO with
 *   duplicate_input_fd. Returns 0 on success or -1 on failure.
 */
int	handle_one_input_redir(t_ms *ms, t_redir *rd)
{
	int	fd;

	fd = get_input_fd(ms, rd);
	if (fd < 0)
		return (-1);
	if (duplicate_input_fd(ms, fd) == -1)
		return (-1);
	return (0);
}
