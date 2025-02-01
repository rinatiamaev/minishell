/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nlouis <nlouis@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/26 16:51:50 by riamaev           #+#    #+#             */
/*   Updated: 2025/01/31 11:13:24 by nlouis           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	export_err(t_ms *ms, const char *arg, char *error_message)
{
	ft_putstr_fd(RED "⭕ Error: " RESET, STDERR_FILENO);
	ft_putstr_fd((char *)arg, STDERR_FILENO);
	ft_putstr_fd(": ", STDERR_FILENO);
	ft_putstr_fd(error_message, STDERR_FILENO);
	ft_putstr_fd("\n", STDERR_FILENO);
	ms->exit_status = 1;
}

static void	process_env_var_with_equal(t_ms *ms, const char *arg)
{
	char		*name;

	name = ft_strndup(arg, ft_strchr(arg, '=') - arg);
	if (!name)
	{
		ms->exit_status = 1;
		error(ms, "malloc() failed");
	}
	if (!is_valid_identifier(name))
	{
		export_err(ms, arg, "not a valid identifier");
		free(name);
		return ;
	}
	free(name);
	add_or_update_env(ms, arg);
	ms->exit_status = 0;
}

static void	process_valid_identifier(t_ms *ms, const char *arg)
{
	char	*existing_var;
	char	*new_var;

	existing_var = getenv(arg);
	if (existing_var)
		return ;
	new_var = ft_strjoin(arg, "=");
	if (!new_var)
		error(ms, "ft_strjoin() failed");
	add_or_update_env(ms, new_var);
	free(new_var);
}

static void	print_env(t_ms *ms)
{
	int	i;

	i = 0;
	while (ms->envp[i])
	{
		printf("declare -x %s\n", ms->envp[i]);
		i++;
	}
	ms->exit_status = 0;
}

void	builtin_export(t_ms *ms, t_cmd *cmd)
{
	int			i;
	const char	*arg;

	i = 0;
	if (!cmd->args || !cmd->args[0])
	{
		print_env(ms);
		return ;
	}
	while (cmd->args[i])
	{
		arg = cmd->args[i];
		if (ft_strchr(arg, '='))
			process_env_var_with_equal(ms, arg);
		else if (is_valid_identifier(arg))
			process_valid_identifier(ms, arg);
		else
			export_err(ms, arg, "not a valid identifier");
		i++;
	}
}
