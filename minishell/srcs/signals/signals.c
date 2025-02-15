/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nlouis <nlouis@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/17 14:53:00 by nlouis            #+#    #+#             */
/*   Updated: 2025/02/15 13:26:05 by nlouis           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* There are two main "modes":
 *	- Interactive: when shell is waiting for user input (Ctrl-C resets prompt)
 *	- Command (non-interactive): when a command is running
 *	(shell ignores signals, letting the child process handle them).
 *
 * ignore_sigquit:
 *   Sets the SIGQUIT signal handler to ignore the signal.
 *   This is used in interactive mode.
 */
static void	ignore_sigquit(t_ms *ms)
{
	struct sigaction	act;

	ft_memset(&act, 0, sizeof(act));
	act.sa_handler = SIG_IGN;
	if (sigaction(SIGQUIT, &act, NULL) == -1)
		error(ms, "sigaction() failed in ingnore_sigquit()");
}

/*
 * interactive_sigint_handler:
 *   Handles SIGINT in interactive mode. Writes a newline and resets
 *   the readline prompt.
 */
static void	interactive_sigint_handler(int signo)
{
	(void)signo;
	write(STDOUT_FILENO, "\n", 1);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}

/*
 * set_signals_interactive:
 *   Configures signal handlers for interactive mode. It ignores SIGQUIT
 *   and sets a custom SIGINT handler.
 */
void	set_signals_interactive(t_ms *ms)
{
	struct sigaction	act;

	ignore_sigquit(ms);
	ft_memset(&act, 0, sizeof(act));
	act.sa_handler = interactive_sigint_handler;
	if (sigaction(SIGINT, &act, NULL) == -1)
		error(ms, "sigaction() failed in set_signals_interactive()");
}

/*
 * cmd_sig_handler:
 *   Handles signals during command execution. Writes a newline on
 *   receiving SIGINT or SIGQUIT.
 */
static void	cmd_sig_handler(int signo)
{
	(void)signo;
	write(STDOUT_FILENO, "\n", 1);
}

/*
 * set_signals_noninteractive:
 *   Configures signal handlers for non-interactive mode (when a command
 *   is running). It sets the custom handler for both SIGINT and SIGQUIT.
 */
void	set_signals_noninteractive(t_ms *ms)
{
	struct sigaction	act;

	ft_memset(&act, 0, sizeof(act));
	act.sa_handler = cmd_sig_handler;
	if (sigaction(SIGINT, &act, NULL) == -1)
		error(ms, "sigaction() failed in set_signals_noninteractive()");
	if (sigaction(SIGQUIT, &act, NULL) == -1)
		error(ms, "sigaction() failed in set_signals_noninteractive()");
}
