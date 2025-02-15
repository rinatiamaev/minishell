/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   setup_ouput_redir.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nlouis <nlouis@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/13 13:27:31 by nlouis            #+#    #+#             */
/*   Updated: 2025/02/15 13:21:06 by nlouis           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
 * open_heredoc_fd:
 *   Returns the heredoc file descriptor stored in rd->heredoc_fd.
 *   If the descriptor is invalid, sets ms->exit_status to 1 and returns -1.
 */
static int	open_heredoc_fd(t_ms *ms, t_redir *rd)
{
	int	fd;

	fd = rd->heredoc_fd;
	if (fd < 0)
	{
		ms->exit_status = 1;
		return (-1);
	}
	return (fd);
}

/*
 * open_file_output_fd:
 *   Opens the output file specified in rd->filename for writing.
 *   If rd->is_append is true, opens the file in append mode;
 *   otherwise, truncates the file. On failure, calls redir_err,
 *   sets ms->exit_status to 1, and returns -1.
 */
static int	open_file_output_fd(t_ms *ms, t_redir *rd)
{
	int	flags;
	int	fd;

	flags = O_WRONLY | O_CREAT;
	if (rd->is_append)
		flags |= O_APPEND;
	else
		flags |= O_TRUNC;
	fd = open(rd->filename, flags, 0644);
	if (fd == -1)
	{
		redir_err(rd, NULL);
		ms->exit_status = 1;
		return (-1);
	}
	return (fd);
}

/*
 * open_output_fd:
 *   Determines the appropriate method to open the output file descriptor.
 *   If the redirection is a heredoc, calls open_heredoc_fd; otherwise,
 *   opens the file with open_file_output_fd.
 */
static int	open_output_fd(t_ms *ms, t_redir *rd)
{
	if (rd->is_heredoc)
		return (open_heredoc_fd(ms, rd));
	else
		return (open_file_output_fd(ms, rd));
}

/*
 * duplicate_output_fd:
 *   Duplicates the file descriptor fd to STDOUT_FILENO using dup2.
 *   On failure, prints an error message, closes fd, sets ms->exit_status
 *   to 1, and returns -1.
 */
static int	duplicate_output_fd(t_ms *ms, int fd)
{
	if (dup2(fd, STDOUT_FILENO) == -1)
	{
		perror("dup2 for output");
		close(fd);
		ms->exit_status = 1;
		return (-1);
	}
	close(fd);
	return (0);
}

/*
 * handle_one_output_redir:
 *   Handles one output redirection by opening the appropriate file descriptor
 *   and duplicating it to STDOUT_FILENO. Returns 0 on success or -1 on failure.
 */
int	handle_one_output_redir(t_ms *ms, t_redir *rd)
{
	int	fd;

	fd = open_output_fd(ms, rd);
	if (fd == -1)
		return (-1);
	return (duplicate_output_fd(ms, fd));
}
