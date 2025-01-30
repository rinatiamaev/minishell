/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: riamaev <riamaev@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/26 14:02:19 by riamaev           #+#    #+#             */
/*   Updated: 2025/01/26 14:08:41 by riamaev          ###   ########.fr       */
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
	(void)target_dir;
	perror("minishell: cd");
	ms->exit_status = 1;
}

static void	update_directory(t_ms *ms, const char *target_dir)
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
	char	*target_dir;
	char	*home_dir;

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
}
