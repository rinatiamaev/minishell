/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nlouis <nlouis@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/31 10:27:16 by nlouis            #+#    #+#             */
/*   Updated: 2025/02/15 12:04:12 by nlouis           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
 * reset_prompt:
 * Cleans up the shell's internal state after each command execution by
 * freeing memory allocated for the command, token list, and input string.
 */
static void	reset_prompt(t_ms *ms)
{
	free_cmd(ms->cmd);
	ms->cmd = NULL;
	free_tks(ms->tks);
	ms->tks = NULL;
	free((char *)ms->input);
	ms->input = NULL;
}

/*
 * setup_minishell:
 * Initializes the shell's environment and configuration.
 * Verifies that no arguments are passed, allocates memory for the shell,
 * and copies the environment variables. Exits on failure.
 */
static t_ms	*setup_minishell(int argc, char **argv, char **envp)
{
	t_ms	*ms;

	if (argc != 1)
	{
		ft_putstr_fd("Wrong format, no arguments expected\n", 2);
		ft_putstr_fd("Usage: ./minishell\n", 2);
		exit(EXIT_FAILURE);
	}
	(void)argv;
	ms = ft_calloc(1, sizeof(t_ms));
	if (!ms)
		error(NULL, "ft_calloc(): malloc failed");
	ms->envp = ft_copy_strarray(envp);
	if (!ms->envp)
		error(ms, "ft_copy_strarray(): malloc failed");
	return (ms);
}

/*
 * main:
 * Entry point of the minishell. Sets up the shell, configures signal
 * handlers, and enters a loop that processes input, executes commands,
 * and resets the shell state for the next input. It resets the prompt
 * if an error occurs during input processing.
 */
int	main(int argc, char **argv, char **envp)
{
	t_ms	*ms;

	ms = setup_minishell(argc, argv, envp);
	set_signals_interactive(ms);
	while (true)
	{
		if (process_input(ms) == -1)
		{
			reset_prompt(ms);
			continue ;
		}
		set_signals_noninteractive(ms);
		execute_cmd(ms, ms->cmd);
		set_signals_interactive(ms);
		reset_prompt(ms);
	}
	return (0);
}
