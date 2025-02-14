/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_word.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nlouis <nlouis@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/13 10:25:51 by nlouis            #+#    #+#             */
/*   Updated: 2025/02/14 14:07:43 by nlouis           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	set_command_name(t_ms *ms, t_cmd *cmd, const char *value)
{
	cmd->name = x_strdup(ms, value);
	if (!cmd->name)
	{
		free_cmd(cmd);
		error(ms, "ft_strdup(): malloc failed");
	}
	cmd->has_command = true;
}

static void	append_command_argument(t_ms *ms, t_cmd *cmd, const char *value)
{
	int		args_len;
	char	**new_args;

	args_len = 0;
	while (cmd->args && cmd->args[args_len])
		args_len++;
	new_args = ft_realloc(cmd->args, args_len * sizeof(char *),
			(args_len + 2) * sizeof(char *));
	if (!new_args)
	{
		free_cmd(cmd);
		error(ms, "ft_realloc(): malloc failed");
	}
	cmd->args = new_args;
	cmd->args[args_len] = x_strdup(ms, value);
	if (!cmd->args[args_len])
	{
		free_cmd(cmd);
		error(ms, "ft_strdup(): malloc failed");
	}
	cmd->args[args_len + 1] = NULL;
}

int	parse_word(t_ms *ms, t_cmd *cmd, t_tk *tk)
{
	if (tk->value[0] == '\0')
		return (0);
	if (!cmd->name)
		set_command_name(ms, cmd, tk->value);
	else
		append_command_argument(ms, cmd, tk->value);
	return (0);
}
