/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_pwd.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: riamaev <riamaev@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/26 18:34:13 by riamaev           #+#    #+#             */
/*   Updated: 2025/01/26 18:37:07 by riamaev          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
		perror("pwd");
		ms->exit_status = 1;
		return ;
	}
	printf("%s\n", cwd);
	free(cwd);
	ms->exit_status = 0;
}
