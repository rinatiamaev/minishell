/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_memory.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nlouis <nlouis@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/02 19:20:21 by nlouis            #+#    #+#             */
/*   Updated: 2025/02/06 22:48:32 by nlouis           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
		{
			free(cmd->args[i]);
			i++;
		}
		free(cmd->args);
	}
	free(cmd->input_redirect);
	free(cmd->output_redirect);
	free(cmd->heredoc_delimiter);
	free(cmd->path);
	if (cmd->pipe_to)
		free_cmd(cmd->pipe_to);
	free(cmd);
}

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

void	free_all(t_ms *ms)
{
	if (!ms)
		return ;
	free((char *)ms->input);
	free_tks(ms->tks);
	free_cmd(ms->cmd);
	ft_free_array((void *)ms->envp);
	rl_clear_history();
	free(ms);
}
