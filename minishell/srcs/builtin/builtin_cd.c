/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nlouis <nlouis@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/26 14:02:19 by riamaev           #+#    #+#             */
/*   Updated: 2025/02/15 13:03:16 by nlouis           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
 * handle_cd_error - Prints an error message for cd failure.
 *
 * This function prints an error message to stderr that includes the
 * command name and, if provided, the target directory. It then sets the
 * exit status to 1.
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
	perror("");
	ms->exit_status = 1;
}

/*
 * update_directory - Changes the current directory and updates env vars.
 *
 * This function attempts to change the directory to target_dir. On success,
 * it updates the OLDPWD and PWD environment variables. If chdir fails,
 * it calls handle_cd_error to report the error.
 */
static void	update_directory(t_ms *ms, const char *target_dir)
{
	char	cwd[1024];
	char	*oldpwd;

	if (chdir(target_dir) == -1)
	{
		handle_cd_error(ms, target_dir);
		return ;
	}
	if (getcwd(cwd, sizeof(cwd)) != NULL)
	{
		oldpwd = getenv("PWD");
		if (oldpwd)
			setenv("OLDPWD", oldpwd, 1);
		setenv("PWD", cwd, 1);
	}
	else
		error(ms, "getcwd() failed");
	ms->exit_status = 0;
}

/*
 * get_target_dir - Retrieves a directory from the environment.
 *
 * This function returns the value of the environment variable specified
 * by var. If the variable is not set, it prints an error message and sets
 * the exit status to 1.
 */
const char	*get_target_dir(t_ms *ms, const char *var)
{
	const char	*target_dir;

	target_dir = getenv(var);
	if (!target_dir)
	{
		if (ft_strcmp(var, "HOME") == 0)
			builtin_err(ms, "HOME not set");
		else
			builtin_err(ms, "OLDPWD not set");
		ms->exit_status = 1;
		return (NULL);
	}
	return (target_dir);
}

/*
 * builtin_cd - Implements the cd builtin command.
 *
 * This function changes the current working directory based on the
 * command arguments. If no argument is provided, it uses the HOME
 * environment variable. If "-" is specified, it switches to OLDPWD and
 * prints the new directory. If more than one argument is given, it reports
 * an error. On success, it calls update_directory to change the directory.
 */
void	builtin_cd(t_ms *ms, t_cmd *cmd)
{
	const char	*target_dir;

	if (!cmd->args || !cmd->args[0])
		target_dir = get_target_dir(ms, "HOME");
	else if (cmd->args[1])
	{
		builtin_err(ms, "too many arguments");
		ms->exit_status = 1;
		return ;
	}
	else if (ft_strcmp(cmd->args[0], "-") == 0)
	{
		target_dir = get_target_dir(ms, "OLDPWD");
		if (!target_dir)
			return ;
		printf("%s\n", target_dir);
	}
	else
		target_dir = cmd->args[0];
	if (target_dir)
		update_directory(ms, target_dir);
}
