/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nlouis <nlouis@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/31 10:27:16 by nlouis            #+#    #+#             */
/*   Updated: 2025/02/04 10:32:43 by nlouis           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	reset_prompt(t_ms *ms)
{
	free_cmd(ms->cmd);
	ms->cmd = NULL;
	free_tks(ms->tks);
	ms->tks = NULL;
	free((char *)ms->input);
	ms->input = NULL;
}

/* setup_minishel()
 *	- validates argc and (void)argv
 *	- allocates memory for ms struct and initializes its fields to 0
 *	- copies environment variables and assigns it to minishell in an array of
 *	strings 'envp' in 'ms' struct
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
	ms = malloc(sizeof(t_ms));
	if (!ms)
		error(NULL, "malloc() failed in setup_minishell()");
	ft_memset(ms, 0, sizeof(t_ms));
	ms->envp = ft_copy_array(envp);
	if (!ms->envp)
		error(ms, "ft_copy_array() failed in setup_minishell()");
	return (ms);
}

/* main()
 *	- set up minishell and assign it to 't_ms *ms'
 *	- continually processes user input, and resets the prompt
 *	at the end of an iteration or if it receives an empty input
 *	or a syntax error occurs
 */
int	main(int argc, char **argv, char **envp)
{
	t_ms	*ms;

	ms = setup_minishell(argc, argv, envp);
	set_signals_interactive();
	while (true)
	{
		if (process_input(ms) == -1)
		{
			set_signals_interactive();
			reset_prompt(ms);
			continue ;
		}
		set_signals_noninteractive();
		execute_cmd(ms, ms->cmd);
		set_signals_interactive();
		reset_prompt(ms);
	}
	return (0);
}
