/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: riamaev <riamaev@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/26 18:05:21 by riamaev           #+#    #+#             */
/*   Updated: 2025/01/26 18:52:56 by riamaev          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
  Checks if a variable name is valid.
  A valid name starts with a letter or '_', 
  followed by alphanumeric or '_' characters.
*/

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

char	*ft_strcpy(char *dst, const char *src)
{
	char	*ret;

	ret = dst;
	while (*src)
		*dst++ = *src++;
	*dst = '\0';
	return (ret);
}

int	find_env_variable(t_ms *ms, const char *var, size_t var_len)
{
	int	i;

	i = 0;
	while (ms->envp[i])
	{
		if (ft_strncmp(ms->envp[i], var, var_len) == 0 && \
			ms->envp[i][var_len] == '=')
			return (i);
		i++;
	}
	return (-1);
}

void	add_or_update_env(t_ms *ms, const char *var)
{
	int		i;
	char	**new_envp;

	i = 0;
	while (ms->envp[i])
		i++;
	new_envp = malloc((i + 2) * sizeof(char *));
	if (!new_envp)
	{
		perror("export");
		exit(EXIT_FAILURE);
	}
	i = 0;
	while (ms->envp[i])
	{
		new_envp[i] = ms->envp[i];
		i++;
	}
	new_envp[i] = ft_strdup(var);
	new_envp[i + 1] = NULL;
	free(ms->envp);
	ms->envp = new_envp;
}
