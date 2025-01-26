/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validate_cmds.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nlouis <nlouis@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/13 17:52:04 by nlouis            #+#    #+#             */
/*   Updated: 2025/01/14 13:18:53 by nlouis           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

static char *get_external_cmd_path(t_ms *ms, const char *cmd_name)
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
        error(ms, "Error ft_split() failed in resolve_external_cmd");
    path = search_executable(path_directories, cmd_name);
    ft_free_array((void **)path_directories);
    return (path);
}

int is_external(t_ms *ms, t_cmd *cmd, char *cmd_name)
{
	char *path;
	
	path = get_external_cmd_path(ms, cmd_name);
	if (path)
	{
		cmd->path = path; 
		return (1);
	}
	return (0);
}

static int	is_builtin(t_cmd *cmd)
{
	int		i;
	char	**builtin_cmds;

	builtin_cmds = (char *[]){
		"echo",
		"cd",
		"pwd",
		"export",
		"unset",
		"env",
		"exit",
		NULL
	};
	i = 0;
	while (builtin_cmds[i])
	{
		if (ft_strcmp(cmd->name, builtin_cmds[i]) == 0)
			return (1);
		i++;
	}
	return (0);
}

static bool	is_cmd_valid(t_ms *ms, t_cmd *cmd)
{
	if (is_builtin(cmd))
	{
		cmd->builtin = true;
		return (true);
	}
	if (is_external(ms, cmd, cmd->name) == 1)
	{
		cmd->builtin = false;
		return (true);
	}
	ms->exit_status = 127;
	return (false);
}

/*validate_cmds()
*
*/
bool	validate_cmds(t_ms *ms, t_cmd *cmd, char **envp)
{
	if (!cmd)
		return (true);
	if (!is_cmd_valid(ms, cmd))
	{
		if (!cmd->builtin && cmd->path && access(cmd->path, X_OK) != 0)
		{
			cmd_err("command not executable");
			ms->exit_status = 126;
		}
		else
		{
			cmd_err("command not found");
			ms->exit_status = 127;
		}
		return (false);
	}
	return (validate_cmds(ms, cmd->pipe_to, envp));
}
