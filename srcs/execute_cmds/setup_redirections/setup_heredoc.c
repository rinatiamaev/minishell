/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   setup_heredoc.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nlouis <nlouis@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/04 14:39:40 by nlouis            #+#    #+#             */
/*   Updated: 2025/02/06 14:15:35 by nlouis           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	read_heredoc_in_pipe(t_ms *ms, int write_fd, char *delimiter,
	bool delimiter_quoted)
{
	char	*line;
	char	*expanded;

	while (true)
	{
		line = readline(BOLD_BLUE "heredoc🔹> " RESET);
		if (!line)
			break ;
		if (!ft_strcmp(line, delimiter))
			break ;
		if (!delimiter_quoted)
		{
			expanded = expand_env_var(ms, line);
			free(line);
			line = expanded;
		}
		write(write_fd, line, ft_strlen(line));
		write(write_fd, "\n", 1);
		free(line);
	}
	free(line);
	close(write_fd);
}

static int	parse_heredocs(t_ms *ms, t_cmd *cmd, t_tk **tks)
{
	int	i;
	int	pipe_fds[2];
	int	prev_fd;

	prev_fd = -1;
	i = -1;
	while (tks[++i])
	{
		if (tks[i]->type == TK_HEREDOC)
		{
			if (pipe(pipe_fds) == -1)
			{
				perror("pipe() for heredoc");
				return (-1);
			}
			read_heredoc_in_pipe(ms, pipe_fds[1], tks[i + 1]->value,
				tks[i + 1]->delimiter_quoted);
			close(pipe_fds[1]);
			prev_fd = pipe_fds[0];
			if (cmd->heredoc_delimiter)
				free(cmd->heredoc_delimiter);
			cmd->heredoc_delimiter = x_strdup(ms, tks[i + 1]->value);
		}
	}
	return (prev_fd);
}

static int	read_heredoc_data_if_no_cmd(t_cmd *cmd, int fd)
{
	char	buf[1024];
	int		bytes;

	if (cmd->name)
		return (1);
	bytes = read(fd, buf, 1023);
	while (bytes > 0)
	{
		buf[bytes] = '\0';
		printf("%s", buf);
		bytes = read(fd, buf, 1023);
	}
	close(fd);
	return (0);
}

static int	connect_heredoc_stdin(int fd)
{
	if (fd == -1)
		return (0);
	if (dup2(fd, STDIN_FILENO) == -1)
	{
		perror("dup2() failed for heredoc");
		close(fd);
		return (-1);
	}
	close(fd);
	return (0);
}

int	handle_heredoc(t_ms *ms, t_cmd *cmd, t_tk **tks, bool is_piped)
{
	int	fd;

	fd = parse_heredocs(ms, cmd, tks);
	if (fd < 0)
		return (-1);
	if (is_piped && !cmd->name)
	{
		close(fd);
		return (0);
	}
	if (!read_heredoc_data_if_no_cmd(cmd, fd))
		return (0);
	return (connect_heredoc_stdin(fd));
}
