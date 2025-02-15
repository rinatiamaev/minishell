/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_unset.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nlouis <nlouis@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/26 13:16:50 by riamaev           #+#    #+#             */
/*   Updated: 2025/02/15 12:51:14 by nlouis           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
 * create_new_envp:
 *   Creates a new environment array that excludes the variable whose key
 *   matches the provided key. The matching entry is freed and not added.
 *   Returns the new environment array.
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

/*
 * remove_env_var:
 *   Removes an environment variable from ms->envp by key.
 *   Replaces the current environment with a new one missing the key.
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

/*
 * builtin_unset:
 *   Implements the unset builtin by removing each key provided in the
 *   command arguments from the environment. Sets exit_status to 0.
 */
void	builtin_unset(t_ms *ms, t_cmd *cmd)
{
	int	i;

	i = 0;
	if (!cmd->args || !cmd->args[0])
		return ;
	while (cmd->args[i])
	{
		remove_env_var(ms, cmd->args[i]);
		i++;
	}
	ms->exit_status = 0;
}
