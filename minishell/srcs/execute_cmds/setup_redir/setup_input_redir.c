/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   setup_input_redir.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nlouis <nlouis@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/13 13:36:26 by nlouis            #+#    #+#             */
/*   Updated: 2025/02/14 10:31:19 by nlouis           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
