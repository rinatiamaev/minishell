/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_unset.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: riamaev <riamaev@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/26 13:16:50 by riamaev           #+#    #+#             */
/*   Updated: 2025/01/26 18:45:22 by riamaev          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	remove_env_var(t_ms *ms, const char *key)
{
	int		i;
	int		j;
	char	**new_envp;
	size_t	key_len;

	i = 0;
	j = 0;
	key_len = ft_strlen(key);
	while (ms->envp[i])
		i++;
	new_envp = malloc(sizeof(char *) * i);
	if (!new_envp)
		perror("malloc() failed in remove_env_var");
	while (ms->envp[i])
	{
		if (ft_strncmp(ms->envp[i], key, key_len) == 0 && \
			ms->envp[i][key_len] == '=')
			free(ms->envp[i]);
		else
			new_envp[j++] = ms->envp[i];
		i++;
	}
	new_envp[j] = NULL;
	free(ms->envp);
	ms->envp = new_envp;
}

void	builtin_unset(t_ms *ms, t_cmd *cmd)
{
	int	i;

	i = 0;
	if (!cmd->args || !cmd->args[0])
	{
		write(2, "unset: not enough arguments\n", 28);
		ms->exit_status = 1;
		return ;
	}
	while (cmd->args[i])
	{
		remove_env_var(ms, cmd->args[i]);
		i++;
	}
	ms->exit_status = 0;
}
