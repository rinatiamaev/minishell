/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   setup_redir.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nlouis <nlouis@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/13 13:21:20 by nlouis            #+#    #+#             */
/*   Updated: 2025/02/14 10:25:05 by nlouis           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	setup_redir(t_ms *ms, t_cmd *cmd)
{
	int	i;

	i = 0;
	while (cmd->input_redirs && cmd->input_redirs[i])
	{
		if (handle_one_input_redir(ms, cmd->input_redirs[i]) == -1)
			return (-1);
		i++;
	}
	i = 0;
	while (cmd->output_redirs && cmd->output_redirs[i])
	{
		if (handle_one_output_redir(ms, cmd->output_redirs[i]) == -1)
			return (-1);
		i++;
	}
	i = 0;
	return (0);
}
