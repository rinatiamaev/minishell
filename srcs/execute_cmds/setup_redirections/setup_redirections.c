/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   setup_redirections.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nlouis <nlouis@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/16 09:12:11 by nlouis            #+#    #+#             */
/*   Updated: 2025/02/03 13:52:36 by nlouis           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

static char	*dequote_delimiter(char *delimiter)
{
	int		len;
	char	*cleaned;

	len = ft_strlen(delimiter);
	if ((delimiter[0] == '\'' && delimiter[len - 1] == '\'')
		|| (delimiter[0] == '"' && delimiter[len - 1] == '"'))
	{
		cleaned = ft_substr(delimiter, 1, len - 2);
		if (!cleaned)
			return (NULL);
		return (cleaned);
	}
	return (ft_strdup(delimiter));
}

static bool	is_quoted_delimiter(char *delimiter)
{
	int len;

	len = ft_strlen(delimiter);
	if (!delimiter || len < 2)
		return (false);
	if ((delimiter[0] == '\'' && delimiter[ft_strlen(delimiter) - 1] == '\'')
		|| (delimiter[0] == '"' && delimiter[ft_strlen(delimiter) - 1] == '"'))
		return (true);
	return (false);
}

static void	read_heredoc_in_pipe(t_ms *ms, int write_fd, char *delimiter)
{
	char	*line;
	char	*expanded_line;
	char	*dequoted;

	dequoted = dequote_delimiter(delimiter);
	if (!dequoted)
	{
		free(delimiter);
		error(ms, "malloc failed in dequote_delimiter");
	}
	while (true)
	{
		line = readline(BOLD_BLUE "heredoc🔹> " RESET);
		if (!line || ft_strcmp(line, dequoted) == 0)
			break ;
		if (!is_quoted_delimiter(delimiter))
		{
			expanded_line = expand_env_var(ms, line);
			free(line);
			line = expanded_line;
		}
		write(write_fd, line, ft_strlen(line));
		write(write_fd, "\n", 1);
		free(line);
	}
	free(line);
	free(dequoted);
	close(write_fd);
}

int	handle_heredoc(t_ms *ms, t_cmd *cmd, t_tk **tks)
{
	int		heredoc_pipe[2];
	int		prev_pipe_fd = -1;
	int		i = 0;

	(void)ms;
	while (tks[i])
	{
		if (tks[i]->type == TK_HEREDOC)
		{
			if (pipe(heredoc_pipe) == -1)
			{
				perror("pipe() for heredoc");
				return (-1);
			}
			read_heredoc_in_pipe(ms, heredoc_pipe[1], tks[i + 1]->value);
			close(heredoc_pipe[1]);
			prev_pipe_fd = heredoc_pipe[0];
			if (cmd->heredoc_delimiter)
			{
				free(cmd->heredoc_delimiter);
				cmd->heredoc_delimiter = NULL;
			}
			cmd->heredoc_delimiter = ft_strdup(tks[i + 1]->value);
		}
		i++;
	}
	if (!cmd->name)
	{
		char buffer[1024];
		int bytes_read;

		while ((bytes_read = read(prev_pipe_fd, buffer, sizeof(buffer) - 1)) > 0)
		{
			buffer[bytes_read] = '\0';
			printf("%s", buffer);
		}
		close(prev_pipe_fd);
		return (0);
	}
	if (prev_pipe_fd != -1)
	{
		if (dup2(prev_pipe_fd, STDIN_FILENO) == -1)
		{
			perror("dup2() failed for heredoc");
			close(prev_pipe_fd);
			return (-1);
		}
		close(prev_pipe_fd);
	}
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

int	setup_redirections(t_ms *ms, t_cmd *cmd)
{
	(void)ms;
	if (cmd->input_redirect)
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
