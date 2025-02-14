/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   setup_heredoc.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nlouis <nlouis@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/13 13:21:25 by nlouis            #+#    #+#             */
/*   Updated: 2025/02/13 13:58:34 by nlouis           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	read_heredoc_in_pipe(t_ms *ms, int write_fd, char *delimiter,
	bool quoted)
{
	char	*line;
	char	*expanded;

	while (true)
	{
		line = readline("heredoc> ");
		if (!line)
			break ;
		if (ft_strcmp(line, delimiter) == 0)
		{
			free(line);
			break ;
		}
		if (!quoted)
		{
			expanded = expand_env_var(ms, line);
			free(line);
			line = expanded;
		}
		write(write_fd, line, ft_strlen(line));
		write(write_fd, "\n", 1);
		free(line);
	}
	close(write_fd);
}

static int	read_one_heredoc(t_ms *ms, t_redir *rd)
{
	int	pipefd[2];

	if (pipe(pipefd) == -1)
	{
		perror("pipe() failed for heredoc");
		return (-1);
	}
	read_heredoc_in_pipe(ms, pipefd[1], rd->delimiter, rd->delimiter_quoted);
	rd->heredoc_fd = pipefd[0];
	return (0);
}

int	read_all_heredocs(t_ms *ms, t_cmd *cmd)
{
	t_cmd	*c;
	int		i;
	t_redir	*rd;

	c = cmd;
	while (c)
	{
		i = 0;
		while (c->input_redirs && c->input_redirs[i])
		{
			rd = c->input_redirs[i];
			if (rd->is_heredoc)
			{
				if (read_one_heredoc(ms, rd) == -1)
					return (-1);
			}
			i++;
		}
		c = c->pipe_to;
	}
	return (0);
}
