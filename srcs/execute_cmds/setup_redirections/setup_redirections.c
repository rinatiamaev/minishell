/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   setup_redirections.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nlouis <nlouis@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/16 09:12:11 by nlouis            #+#    #+#             */
/*   Updated: 2025/02/01 18:55:13 by nlouis           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

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

static int	handle_heredoc(t_ms *ms, t_cmd *cmd)
{
	int	heredoc_pipe[2];

	(void)ms;
	if (pipe(heredoc_pipe) == -1)
	{
		perror("pipe() for heredoc");
		return (-1);
	}
	read_heredoc_into_pipe(heredoc_pipe[1], cmd->heredoc_delimiter);
	if (dup2(heredoc_pipe[0], STDIN_FILENO) == -1)
	{
		perror("dup2(heredoc_pipe[0])");
		close(heredoc_pipe[0]);
		return (-1);
	}
	close(heredoc_pipe[0]);
	return (0);
}

static int	handle_output_redirection(t_cmd *cmd)
{
	int	out_fd;
	int	flags;

	flags = O_WRONLY | O_CREAT;
	if (cmd->append)
		flags |= O_APPEND;
	else
		flags |= O_TRUNC;
	out_fd = open(cmd->output_redirect, flags, FILE_PERMISSIONS);
	if (out_fd == -1)
	{
		perror("Error opening output file");
		return (-1);
	}
	if (dup2(out_fd, STDOUT_FILENO) == -1)
	{
		perror("Error duplicating output file descriptor");
		close(out_fd);
		return (-1);
	}
	close(out_fd);
	return (0);
}

static int	handle_input_redirection(t_cmd *cmd)
{
	int	in_fd;

	in_fd = open(cmd->input_redirect, O_RDONLY);
	if (in_fd == -1)
	{
		perror("Error opening input file");
		return (-1);
	}
	if (dup2(in_fd, STDIN_FILENO) == -1)
	{
		perror("Error duplicating input file descriptor");
		close(in_fd);
		return (-1);
	}
	close(in_fd);
	return (0);
}

int	setup_redirections(t_cmd *cmd)
{
	if (cmd->heredoc_delimiter)
	{
		if (handle_heredoc(NULL, cmd) == -1)
			return (-1);
	}
	else if (cmd->input_redirect)
	{
		if (handle_input_redirection(cmd) == -1)
			return (-1);
	}
	if (cmd->output_redirect)
	{
		if (handle_output_redirection(cmd) == -1)
			return (-1);
	}
	return (0);
}
