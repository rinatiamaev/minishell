/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_memory.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nlouis <nlouis@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/02 19:20:21 by nlouis            #+#    #+#             */
/*   Updated: 2025/02/15 13:10:49 by nlouis           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
 * free_redirs:
 *   Frees an array of redirection structures. For each redirection,
 *   frees the filename and delimiter, closes heredoc_fd if open,
 *   and then frees the redirection structure. Finally, frees the array.
 */
void	free_redirs(t_redir **redirs)
{
	int	i;

	if (!redirs)
		return ;
	i = 0;
	while (redirs[i])
	{
		free(redirs[i]->filename);
		free(redirs[i]->delimiter);
		if (redirs[i]->heredoc_fd != -1)
			close(redirs[i]->heredoc_fd);
		free(redirs[i]);
		i++;
	}
	free(redirs);
}

/*
 * free_cmd:
 *   Recursively frees a command structure. Frees the command name,
 *   all arguments, input and output redirections, command path, and
 *   recursively frees any piped commands.
 */
void	free_cmd(t_cmd *cmd)
{
	int	i;

	if (!cmd)
		return ;
	free(cmd->name);
	if (cmd->args)
	{
		i = 0;
		while (cmd->args[i])
			free(cmd->args[i++]);
		free(cmd->args);
	}
	free_redirs(cmd->input_redirs);
	free_redirs(cmd->output_redirs);
	free(cmd->path);
	if (cmd->pipe_to)
		free_cmd(cmd->pipe_to);
	free(cmd);
}

/*
 * free_tks:
 *   Frees an array of tokens. For each token, frees its value and
 *   the token itself, then frees the token array.
 */
void	free_tks(t_tk **tks)
{
	int	i;

	if (!tks)
		return ;
	i = 0;
	while (tks[i])
	{
		free(tks[i]->value);
		free(tks[i]);
		i++;
	}
	free(tks);
}

/*
 * free_all:
 *   Frees all allocated memory in the minishell structure. Frees the
 *   input string, token array, command structure, environment array,
 *   clears the readline history, and finally frees the minishell struct.
 */
void	free_all(t_ms *ms)
{
	if (!ms)
		return ;
	free(ms->input);
	free_tks(ms->tks);
	free_cmd(ms->cmd);
	ft_free_array((void **)ms->envp);
	rl_clear_history();
	free(ms);
}
