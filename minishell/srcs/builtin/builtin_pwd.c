/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_pwd.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nlouis <nlouis@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/26 18:34:13 by riamaev           #+#    #+#             */
/*   Updated: 2025/02/15 12:54:03 by nlouis           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
 * pwd_err:
 *   Handles errors when retrieving the current working directory.
 *   Prints an error message and sets the exit status to 1.
 */
static void	pwd_err(t_ms *ms)
{
	ft_putstr_fd(RED "⭕ Error: " RESET, STDERR_FILENO);
	ft_putstr_fd(ms->cmd->name, STDERR_FILENO);
	ft_putstr_fd(": ", STDERR_FILENO);
	perror("");
	ms->exit_status = 1;
}

/*
 * builtin_pwd:
 *   Implements the pwd builtin command. It retrieves and prints the
 *   current working directory. On failure, it calls pwd_err().
 */
void	builtin_pwd(t_ms *ms)
{
	char	*cwd;

	cwd = getcwd(NULL, 0);
	if (!cwd)
	{
		pwd_err(ms);
		return ;
	}
	printf("%s\n", cwd);
	free(cwd);
	ms->exit_status = 0;
}
