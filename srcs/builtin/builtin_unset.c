/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_unset.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nlouis <nlouis@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/26 13:16:50 by riamaev           #+#    #+#             */
/*   Updated: 2025/01/31 12:59:12 by nlouis           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* create_new_envp():
 *	- Allocates a new environment array.
 *	- Copies over all variables except the one to be removed.
 *	- Returns the new environment array.
 */
static char	**create_new_envp(t_ms *ms, const char *key, size_t key_len)
{
	int		i;
	int		j;
	int		total;
	char	**new_envp;

	total = 0;
	while (ms->envp[total])
		total++;
	new_envp = malloc(sizeof(char *) * (total + 1));
	if (!new_envp)
		error(ms, "malloc() failed");
	i = 0;
	j = 0;
	while (ms->envp[i])
	{
		if (ft_strncmp(ms->envp[i], key, key_len) == 0 
			&& ms->envp[i][key_len] == '=')
			free(ms->envp[i]);
		else
			new_envp[j++] = ms->envp[i];
		i++;
	}
	new_envp[j] = NULL;
	return (new_envp);
}

/**
 * remove_env_var:
 *  - Finds and removes an environment variable by key.
 *  - Calls `create_new_envp` to allocate a new env list.
 *  - Frees the old env list and replaces `ms->envp`.
 */
static void	remove_env_var(t_ms *ms, const char *key)
{
	size_t	key_len;
	char	**new_envp;

	if (!ms->envp || !key)
		return ;
	key_len = ft_strlen(key);
	new_envp = create_new_envp(ms, key, key_len);
	free(ms->envp);
	ms->envp = new_envp;
}

void	builtin_unset(t_ms *ms, t_cmd *cmd)
{
	int	i;

	if (!cmd->args || !cmd->args[0])
	{
		builtin_err(ms, "not enough arguments");
		ms->exit_status = 1;
		return ;
	}
	i = 0;
	while (cmd->args[i])
	{
		remove_env_var(ms, cmd->args[i]);
		i++;
	}
	ms->exit_status = 0;
}
