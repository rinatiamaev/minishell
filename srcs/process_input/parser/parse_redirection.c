/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_redirection.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nlouis <nlouis@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/28 09:43:40 by nlouis            #+#    #+#             */
/*   Updated: 2025/02/05 14:18:41 by nlouis           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* parse_heredoc()
 *	- advances the tk index (*i) past the `<<` tk.
 *	- validates the presence of the next tk:
 *		- if no tk exists, reports a syntax error
 *	- validates the type of the next tk:
 *		- if the tk is not a `tk_WORD`, reports a syntax error
 *	- if a valid delimiter tk is found:
 *		- copies the value of the tk (the heredoc delimiter) into
 *		`cmd->heredoc_delimiter`
 *	- returns: `0` on success, `-1` if a syntax error occurs
 */
static int	parse_heredoc(t_ms *ms, t_cmd *cmd, t_tk **tks, int *i)
{
	(*i)++;
	if (!tks[*i])
	{
		free_cmd(cmd);
		syn_err(ms, "expected a delimiter after '<<'");
		return (-1);
	}
	if (tks[*i]->type != TK_HEREDOC_DELIMITER)
	{
		free_cmd(cmd);
		syn_err(ms, "invalid tk after '<<'");
		return (-1);
	}
	free(cmd->heredoc_delimiter);
	cmd->heredoc_delimiter = ft_strdup(tks[*i]->value);
	if (!cmd->heredoc_delimiter)
	{
		free_cmd(cmd);
		error(ms, "ft_strdup(): malloc failed");
	}
	return (0);
}

/* parse_redirect_output()
 *	- advances the tk index (*i) past the redirection tk (`>` or `>>`).
 *	- checks for the next tk:
 *		- if no tk exists, reports a syntax error
 *		- if the next tk is not a `tk_WORD`, reports a syntax error
 *	- if a valid tk is found:
 *		- copies the value of the tk (the file name) into
 *		`cmd->output_redirect` using `ft_strdup`
 *		- sets the `cmd->append` flag to the provided `append` value
 *	- returns: `0` on success, `-1` if a syntax error occurs.
 */
static int	parse_redirect_output(t_ms *ms, t_cmd *cmd, int *i, int append)
{
	(*i)++;
	if (!ms->tks[*i])
	{
		free_cmd(cmd);
		syn_err(ms, "expected a file after '>' or '>>'");
		return (-1);
	}
	if (ms->tks[*i]->type != TK_WORD)
	{
		free_cmd(cmd);
		syn_err(ms, "invalid tk after '>' or '>>'");
		return (-1);
	}
	cmd->output_redirect = ft_strdup(ms->tks[*i]->value);
	if (!cmd->output_redirect)
	{
		free_cmd(cmd);
		error(ms, "ft_strdup(): malloc failed");
	}
	cmd->append = append;
	return (0);
}

/* parse_redirect_input()
 *	- advances the tk index (*i) past the `<` tk.
 *	- checks for the next tk:
 *		- if no tk exists, reports a syntax error
 *		- if the next tk is not a `tk_WORD`, reports a syntax error
 *	-if a valid tk is found:
 *		- copies the value of the tk (the file name) into
 *		`cmd->input_redirect` using `ft_strdup`
 *	- returns: `0` on success, `-1` if a syntax error occurs
 */
static int	parse_redirect_input(t_ms *ms, t_cmd *cmd, t_tk **tks, int *i)
{
	(*i)++;
	if (!tks[*i])
	{
		free_cmd(cmd);
		syn_err(ms, "expected a file after '<'");
		return (-1);
	}
	if (tks[*i]->type != TK_WORD)
	{
		free_cmd(cmd);
		syn_err(ms, "invalid tk after '<'");
		return (-1);
	}
	cmd->input_redirect = ft_strdup(tks[*i]->value);
	if (!cmd->input_redirect)
	{
		free_cmd(cmd);
		error(ms, "ft_strdup(): malloc failed");
	}
	return (0);
}

/**
 * parse_redirections()
 *	- checks the type of the current tk:
 *		- `tk_REDIRECT_INPUT` (`<`): Parses input redirection and updates
 *		`cmd->input_redirect`.
 *		- `tk_REDIRECT_OUTPUT` (`>`): Parses output redirection and updates
 *		`cmd->output_redirect`.
 *		- `tk_APPEND_OUTPUT` (`>>`): Parses append output redirection and
 *		updates `cmd->output_redirect` with `append` mode enabled.
 *		- `tk_HEREDOC` (`<<`): Parses heredoc redirection and updates
 *		`cmd->heredoc_delimiter`.
 *	- calls the appropriate parsing function based on the tk type:
 *    - `parse_redirect_input()`, `parse_redirect_output()`, `parse_heredoc()`
 *	- advances the tk index (`*i`) within the called parsing functions
 *	- returns: `0` on success, `-1` if asyntax error occurs during parsing
 *	(missing or invalid tk after a redirection operator)
 */
int	parse_redirections(t_ms *ms, t_cmd *cmd, t_tk **tks, int *i)
{
	if (tks[*i]->type == TK_REDIRECT_INPUT)
		return (parse_redirect_input(ms, cmd, tks, i));
	if (tks[*i]->type == TK_REDIRECT_OUTPUT)
		return (parse_redirect_output(ms, cmd, i, 0));
	if (tks[*i]->type == TK_APPEND_OUTPUT)
		return (parse_redirect_output(ms, cmd, i, 1));
	if (tks[*i]->type == TK_HEREDOC)
		return (parse_heredoc(ms, cmd, tks, i));
	return (0);
}
