/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_heredoc_redirection.c                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nlouis <nlouis@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/28 09:14:20 by nlouis            #+#    #+#             */
/*   Updated: 2025/01/30 12:30:29 by nlouis           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	create_temp_file(void)
{
	int	temp_fd;
	int	flags;

	flags = O_WRONLY | O_CREAT | O_TRUNC;
	temp_fd = open(".here_doc_temp", flags, FILE_PERMISSIONS);
	if (temp_fd == -1)
		perror("Error creating temporary file");
	return (temp_fd);
}

static void	read_until_limiter(int temp_fd, const char *limiter)
{
	char	*line;

	while (1)
	{
		line = readline(BOLD_BLUE "heredoc🔹> " RESET);
		if (!line || ft_strcmp(line, limiter) == 0)
		{
			free(line);
			break ;
		}
		write(temp_fd, line, ft_strlen(line));
		write(temp_fd, "\n", 1);
		free(line);
	}
}

static int	open_temp_file_for_reading(void)
{
	int	temp_fd;

	temp_fd = open(".here_doc_temp", O_RDONLY);
	if (temp_fd == -1)
	{
		perror("Error opening temporary file");
		unlink(".here_doc_temp");
	}
	return (temp_fd);
}

static int	setup_heredoc(t_cmd *cmd)
{
	int	temp_fd;
	int	in_fd;

	if (!cmd->heredoc_delimiter)
		return (-1);
	temp_fd = create_temp_file();
	if (temp_fd == -1)
		return (-1);
	read_until_limiter(temp_fd, cmd->heredoc_delimiter);
	close(temp_fd);
	in_fd = open_temp_file_for_reading();
	if (in_fd == -1)
		return (-1);
	if (!cmd->pipe_to)
		unlink(".here_doc_temp");
	return (in_fd);
}

int	handle_heredoc_redirection(t_cmd *cmd)
{
	int	in_fd;

	in_fd = setup_heredoc(cmd);
	if (in_fd == -1)
	{
		perror("Error opening input file for heredoc");
		return (-1);
	}
	if (dup2(in_fd, STDIN_FILENO) == -1)
	{
		perror("Error duplicating heredoc file descriptor");
		close(in_fd);
		return (-1);
	}
	close(in_fd);
	return (0);
}
