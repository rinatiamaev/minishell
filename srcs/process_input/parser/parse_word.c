/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_word.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nlouis <nlouis@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/04 14:58:46 by nlouis            #+#    #+#             */
/*   Updated: 2025/02/05 14:17:14 by nlouis           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	parse_word_set_name(t_ms *ms, t_cmd *cmd, t_tk *tk)
{
	cmd->name = ft_strdup(tk->value);
	if (!cmd->name)
	{
		free_cmd(cmd);
		error(ms, "ft_strdup(): malloc failed");
	}
}

static void	parse_word_add_arg(t_ms *ms, t_cmd *cmd, t_tk *tk)
{
	int		args_len;
	char	**new_args;

	args_len = 0;
	while (cmd->args && cmd->args[args_len])
		args_len++;
	new_args = ft_realloc(cmd->args, args_len * sizeof(char *),
			sizeof(char *) * (args_len + 2));
	if (!new_args)
	{
		free_cmd(cmd);
		error(ms, "ft_realloc(): malloc failed");
	}
	cmd->args = new_args;
	cmd->args[args_len] = ft_strdup(tk->value);
	if (!cmd->args[args_len])
	{
		free_cmd(cmd);
		error(ms, "ft_strdup(): malloc failed");
	}
	cmd->args[args_len + 1] = NULL;
}

void	parse_word(t_ms *ms, t_cmd *cmd, t_tk *tk)
{
	if (!cmd->name)
		parse_word_set_name(ms, cmd, tk);
	else
		parse_word_add_arg(ms, cmd, tk);
}
