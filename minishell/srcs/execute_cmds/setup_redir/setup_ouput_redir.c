/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   setup_ouput_redir.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nlouis <nlouis@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/13 13:27:31 by nlouis            #+#    #+#             */
/*   Updated: 2025/02/14 10:31:27 by nlouis           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

static int	open_output_fd(t_ms *ms, t_redir *rd)
{
	if (rd->is_heredoc)
		return (open_heredoc_fd(ms, rd));
	else
		return (open_file_output_fd(ms, rd));
}

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

int	handle_one_output_redir(t_ms *ms, t_redir *rd)
{
	int	fd;

	fd = open_output_fd(ms, rd);
	if (fd == -1)
		return (-1);
	return (duplicate_output_fd(ms, fd));
}
