/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_pwd.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nlouis <nlouis@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/26 18:34:13 by riamaev           #+#    #+#             */
/*   Updated: 2025/01/31 12:35:29 by nlouis           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	pwd_err(t_ms *ms)
{
	ft_putstr_fd(RED "⭕ Error: " RESET, STDERR_FILENO);
	ft_putstr_fd(ms->cmd->name, STDERR_FILENO);
	ft_putstr_fd(":\n", STDERR_FILENO);
	perror("");
	ms->exit_status = 1;
}

/*
builtin_pwd function prints the current working directory.
It uses getcwd to retrieve the directory, handles errors with perror,
and frees memory before exiting
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
