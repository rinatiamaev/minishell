/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nlouis <nlouis@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/17 14:53:00 by nlouis            #+#    #+#             */
/*   Updated: 2025/02/04 11:25:50 by nlouis           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* There are two main "modes":
 *	- Interactive: when shell is waiting for user input (Ctrl-C resets prompt)
 *	- Command (non-interactive): when a command is running
 *	(shell ignores signals, letting the child process handle them).
 */

/* ignore_sigquit()
 *	Explicitly sets SIGQUIT (Ctrl-\) to be ignored by this process.
 *	Typically used in interactive mode so the shell doesn't quit on Ctrl-\.
 */
static void	ignore_sigquit(t_ms *ms)
{
	struct sigaction	act;

	ft_memset(&act, 0, sizeof(act));
	act.sa_handler = SIG_IGN;
	if (sigaction(SIGQUIT, &act, NULL) == -1)
		error(ms, "sigaction() failed in ingnore_sigquit()");
}

/* interactive_sigint_handler()
 *	Called when user presses Ctrl-C in interactive mode.
 *	Prints a newline, clears the current input line, and re-displays prompt.
 */
static void	interactive_sigint_handler(int signo)
{
	(void)signo;
	write(STDOUT_FILENO, "\n", 1);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}

/* set_signals_interactive()
 *	In interactive mode:
 *		- SIGINT (Ctrl-C) resets the prompt
 *		- SIGQUIT (Ctrl-\) is ignored
 *	This mode is enabled when minishell is awaiting user commands.
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

/* cmd_sig_handler:
 *	Handler for Ctrl-C or Ctrl-\ in "command" mode (non-interactive).
 *	The child process (command) can receive and handle signals on its own.
 */
static void	cmd_sig_handler(int signo)
{
	(void)signo;
	write(STDOUT_FILENO, "\n", 1);
}

/* set_signals_noninteractive:
 *	In "non-interactive" mode for the shell, a command is running.
 *	Let the child process handle Ctrl-C, Ctrl-\ signals,
 *	the shell does not react.
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
