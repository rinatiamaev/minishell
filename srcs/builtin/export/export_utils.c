/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nlouis <nlouis@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/26 18:05:21 by riamaev           #+#    #+#             */
/*   Updated: 2025/01/31 12:22:06 by nlouis           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	is_valid_identifier(const char *name)
{
	int	i;

	i = 1;
	if (!name || (!ft_isalpha(name[0]) && name[0] != '_'))
		return (false);
	while (name[i])
	{
		if (!ft_isalnum(name[i]) && name[i] != '_')
			return (false);
		i++;
	}
	return (true);
}

int	find_env_variable(t_ms *ms, const char *var, size_t var_len)
{
	int	i;

	i = 0;
	while (ms->envp[i])
	{
		if (ft_strncmp(ms->envp[i], var, var_len) == 0
			&& ms->envp[i][var_len] == '=')
			return (i);
		i++;
	}
	return (-1);
}

static void append_new_env(t_ms *ms, const char *var)
{
	int		i;
	char	**new_envp;

	i = 0;
	while (ms->envp[i])
		i++;
	new_envp = malloc(sizeof(char *) * (i + 2));
	if (!new_envp)
		error(ms, "malloc() failed");
	i = 0;
	while (ms->envp[i])
	{
		new_envp[i] = ms->envp[i];
		i++;
	}
	new_envp[i] = ft_strdup(var);
	if (!new_envp[i])
		error(ms, "ft_strdup() failed");
	new_envp[i + 1] = NULL;
	free(ms->envp);
	ms->envp = new_envp;
}

static int update_existing_env(t_ms *ms, const char *var)
{
	char	*equals_sign;
	int		var_len;
	int		index;

	equals_sign = ft_strchr(var, '=');
	if (equals_sign)
		var_len = equals_sign - var;
	else
		var_len = ft_strlen(var);
	index = find_env_variable(ms, var, var_len);
	if (index != -1)
	{
		free(ms->envp[index]);
		ms->envp[index] = ft_strdup(var);
		return (1);
	}
	return (0);
}

void	add_or_update_env(t_ms *ms, const char *var)
{
	if (update_existing_env(ms, var) == 1)
		return ;
	append_new_env(ms, var);
}
