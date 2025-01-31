/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nlouis <nlouis@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/26 14:02:19 by riamaev           #+#    #+#             */
/*   Updated: 2025/01/31 10:37:43 by nlouis           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
builtin_cd function sets the target directory, 
either from cmd->args or the HOME environment variable. 
It calls update_directory, which attempts to change directory using chdir. 
If chdir fails, handle_cd_error reports error; 
if successful, it prints current directory.
*/

static void	handle_cd_error(t_ms *ms, const char *target_dir)
{
	ft_putstr_fd(RED "⭕ Error: " RESET, STDERR_FILENO);
	ft_putstr_fd(ms->cmd->name, STDERR_FILENO);
	ft_putstr_fd(": ", STDERR_FILENO);
	if (target_dir)
	{
		ft_putstr_fd((char *)target_dir, STDERR_FILENO);
		ft_putstr_fd(": ", STDERR_FILENO);
	}
	if (errno)
		perror("");
	else
		ft_putstr_fd("missing argument\n", STDERR_FILENO);
	ms->exit_status = 1;
}

// MAXIMALIST IMPLEMENTATION
static void	update_directory(t_ms *ms, const char *target_dir)
{
	char	cwd[1024];

	if (chdir(target_dir) == -1)
	{
		handle_cd_error(ms, target_dir);
		return ;
	}
	if (getcwd(cwd, sizeof(cwd)) != NULL)
	{
		setenv("OLDPWD", getenv("PWD"), 1);
		setenv("PWD", cwd, 1);
	}
	else
		error(ms, "getcwd() failed");
	ms->exit_status = 0;
}

const char	*get_target_dir(t_ms *ms, const char *target_dir, char *var)
{
	target_dir = getenv(var);
	if (!target_dir)
	{
		if (ft_strcmp(var, "HOME") == 0)
			builtin_err(ms, "HOME not set");
		if (ft_strcmp(var, "OLDPWD") == 0)
			builtin_err(ms, "OLDPWD not set");
		ms->exit_status = 1;
		return (NULL);
	}
	return (target_dir);
}

void	builtin_cd(t_ms *ms, t_cmd *cmd)
{
	const char	*target_dir;

	if (!cmd->args || !cmd->args[0])
	{
		target_dir = get_target_dir(ms, target_dir, "HOME");
		if (!target_dir)
			return ;
	}
	else if (ft_strcmp(cmd->args[0], "-") == 0)
	{
		target_dir = get_target_dir(ms, target_dir, "OLDPWD");
		if (!target_dir)
			return ;
		printf("%s\n", target_dir);
	}
	else
		target_dir = cmd->args[0];
	update_directory(ms, target_dir);
}

// MINIMALIST IMPLEMENTATION
/* void	builtin_cd(t_ms *ms, t_cmd *cmd)
{
	if (!cmd->args || !cmd->args[0])
	{
		handle_cd_error(ms, NULL);
		ms->exit_status = 1;
		return ;
	}
	if (chdir(cmd->args[0]) == -1)
	{
		handle_cd_error(ms, (const char *)cmd->args[0]);
		return ;
	}
	ms->exit_status = 0;
} */
// RINNAT IMPLEMENTATION
/* static void	update_directory(t_ms *ms, const char *target_dir)
{
	char	cwd[1024];

	if (chdir(target_dir) == -1)
		handle_cd_error(ms, target_dir);
	else
	{
		ms->exit_status = 0;
		if (getcwd(cwd, sizeof(cwd)) != NULL)
			printf("Current directory: %s\n", cwd);
		else
			perror("getcwd() failed");
	}
}
void	builtin_cd(t_ms *ms, t_cmd *cmd)
{
	const char	*target_dir;
	char		*home_dir;

	target_dir = NULL;
	home_dir = NULL;
	if (cmd && cmd->args && cmd->args[0])
		target_dir = cmd->args[0];
	else
	{
		home_dir = getenv("HOME");
		if (!home_dir)
		{
			printf("minishell: cd: HOME not set\n");
			ms->exit_status = 1;
			return ;
		}
		target_dir = home_dir;
	}
	printf("cd to: %s\n", target_dir);
	update_directory(ms, target_dir);
} */
