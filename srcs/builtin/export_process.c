/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_process.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: riamaev <riamaev@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/26 18:05:28 by riamaev           #+#    #+#             */
/*   Updated: 2025/01/26 18:05:30 by riamaev          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	process_env_var_with_equal(t_ms *ms, const char *arg)
{
	char		*name;

	name = ft_strndup(arg, ft_strchr(arg, '=') - arg);
	if (!is_valid_identifier(name))
	{
		write(2, "minishell: export: `", 19);
		write(2, arg, ft_strlen(arg));
		write(2, "`: not a valid identifier\n", 25);
		free(name);
		ms->exit_status = 1;
		return ;
	}
	free(name);
	add_or_update_env(ms, arg);
	ms->exit_status = 0;
}

void	process_valid_identifier(t_ms *ms, const char *arg)
{
	char	*placeholder;

	placeholder = malloc(strlen(arg) + 2);
	if (!placeholder)
	{
		perror("export");
		exit(EXIT_FAILURE);
	}
	ft_strcpy(placeholder, arg);
	placeholder[ft_strlen(arg)] = '=';
	placeholder[ft_strlen(arg) + 1] = '\0';
	add_or_update_env(ms, placeholder);
	free(placeholder);
}
