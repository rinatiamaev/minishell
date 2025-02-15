/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   setup_heredoc.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nlouis <nlouis@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/13 13:21:25 by nlouis            #+#    #+#             */
/*   Updated: 2025/02/15 13:18:59 by nlouis           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
 * read_heredoc_in_pipe:
 *   Reads user input for a heredoc, writing each line into a pipe.
 *   The input is read until the delimiter is encountered.
 *   If the heredoc is not quoted, environment variables in the line
 *   are expanded. Each line is written to the pipe followed by a newline.
 */
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

/*
 * read_one_heredoc:
 *   Sets up a pipe for a single heredoc and calls read_heredoc_in_pipe to
 *   read the heredoc contents from the user. The read end of the pipe is
 *   stored in rd->heredoc_fd for later use.
 */
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

/*
 * read_all_heredocs:
 *   Iterates through all commands in the pipeline and processes their
 *   input redirections. For each heredoc redirection, it calls read_one_heredoc
 *   to set up the heredoc content in a pipe.
 */
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
