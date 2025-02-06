/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   is_external.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nlouis <nlouis@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/28 08:42:03 by nlouis            #+#    #+#             */
/*   Updated: 2025/02/06 14:43:13 by nlouis           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

static char	*search_executable(char **path_directories, const char *cmd)
{
	int		i;
	char	*temp;
	char	*candidate;

	i = 0;
	while (path_directories[i])
	{
		temp = ft_strjoin(path_directories[i], "/");
		candidate = ft_strjoin(temp, cmd);
		free(temp);
		if (access(candidate, X_OK) == 0)
			return (candidate);
		free(candidate);
		i++;
	}
	return (NULL);
}

static char	*get_envp_path(char **envp)
{
	while (*envp)
	{
		if (ft_strncmp("PATH=", *envp, 5) == 0)
			return (*envp + 5);
		envp++;
	}
	return (NULL);
}

static char	*check_absolute_path(char *cmd)
{
	if (ft_strchr(cmd, '/') && access(cmd, X_OK) == 0)
		return (ft_strdup(cmd));
	return (NULL);
}

static char	*get_external_cmd_path(t_ms *ms, const char *cmd_name)
{
	char	*path;
	char	*envp_path;
	char	**path_directories;

	path = check_absolute_path((char *)cmd_name);
	if (path)
		return (path);
	envp_path = get_envp_path(ms->envp);
	if (!envp_path)
		return (NULL);
	path_directories = ft_split(envp_path, ':');
	if (!path_directories)
		error(ms, "ft_split(): malloc failed");
	path = search_executable(path_directories, cmd_name);
	ft_free_array((void **)path_directories);
	return (path);
}

bool	is_external(t_ms *ms, t_cmd *cmd, char *cmd_name)
{
	char	*path;

	path = get_external_cmd_path(ms, cmd_name);
	if (path)
	{
		cmd->path = path;
		cmd->builtin = false;
		return (true);
	}
	return (false);
}
