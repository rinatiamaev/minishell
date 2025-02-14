/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_tks_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nlouis <nlouis@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/13 10:37:47 by nlouis            #+#    #+#             */
/*   Updated: 2025/02/13 10:49:28 by nlouis           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_cmd	*new_cmd(t_ms *ms, t_cmd *head)
{
	t_cmd	*cmd;

	cmd = malloc(sizeof(t_cmd));
	if (!cmd)
	{
		if (head)
			free_cmd(head);
		error(ms, "new_cmd(): malloc failed");
	}
	ft_memset(cmd, 0, sizeof(t_cmd));
	return (cmd);
}
