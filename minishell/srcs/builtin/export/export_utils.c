/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nlouis <nlouis@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/26 18:05:21 by riamaev           #+#    #+#             */
/*   Updated: 2025/02/15 13:06:14 by nlouis           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
 * is_valid_identifier:
 *   Checks if a given name is a valid identifier. The first character
 *   must be alphabetic or an underscore, and subsequent characters must
 *   be alphanumeric or an underscore.
 */
bool	is_valid_identifier(char *name)
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

/*
 * find_env_variable:
 *   Searches for an environment variable in ms->envp that matches var.
 *   var_len is the length of the variable name. Returns the index if
 *   found, or -1 if not found.
 */
int	find_env_variable(t_ms *ms, char *var, size_t var_len)
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

/*
 * append_new_env:
 *   Appends a new environment variable to ms->envp. Allocates a new array
 *   containing the existing variables plus the new one.
 */
static void	append_new_env(t_ms *ms, char *var)
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

/*
 * update_existing_env:
 *   Updates an existing environment variable in ms->envp with a new value.
 *   It computes the length of the variable name, searches for it, frees the
 *   old entry, and duplicates the new string. Returns 1 if updated,
 *   or 0 if the variable was not found.
 */
static int	update_existing_env(t_ms *ms, char *var)
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
		if (!ms->envp[index])
			error(ms, "ft_strdup() failed");
		return (1);
	}
	return (0);
}

/*
 * add_or_update_env:
 *   Adds a new environment variable or updates an existing one.
 *   If update_existing_env returns 1, the variable is updated;
 *   otherwise, it is appended to ms->envp.
 */
void	add_or_update_env(t_ms *ms, char *var)
{
	if (update_existing_env(ms, var) == 1)
		return ;
	append_new_env(ms, var);
}
