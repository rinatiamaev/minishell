/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debugging_print.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nlouis <nlouis@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/05 00:34:19 by nlouis            #+#    #+#             */
/*   Updated: 2025/02/10 14:13:45 by nlouis           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void print_indentation(int indent)
{
	for (int i = 0; i < indent; i++)
		printf("    ");
}

void print_cmd(t_cmd *cmd, int indent)
{
	if (!cmd)
	{
		print_indentation(indent);
		printf("cmd: (null)\n");
		return;
	}

	print_indentation(indent);
	printf("cmd: %s\n", cmd->name ? cmd->name : "(none)");

	print_indentation(indent);
	if (cmd->args)
	{
		printf("Arguments:\n");
		for (int i = 0; cmd->args[i]; i++)
		{
			print_indentation(indent + 1); 
			printf("- %s\n", cmd->args[i]);
		}
	}
	else
	{
		printf("Arguments: (none)\n");
	}

	print_indentation(indent);
	printf("Input redirection: %s\n", 
		cmd->input_redirect ? cmd->input_redirect : "(none)");

	print_indentation(indent);
	if (cmd->output_redirect)
		printf("Output redirection: %s (%s mode)\n", 
			cmd->output_redirect, cmd->append ? "append" : "overwrite");
	else
		printf("Output redirection: (none)\n");

	if (cmd->pipe_to)
	{
		print_indentation(indent);
		printf("Pipe to:\n");
		print_cmd(cmd->pipe_to, indent + 1);
	}
	else
	{
		print_indentation(indent);
		printf("Pipe: (none)\n");
	}
}


void print_tks(t_tk **tks)
{
	int i = 0;
	
	if (!tks)
	{
		printf("tks: (none)\n");
		return;
	}
	printf("tks:\n");
	while (tks[i])
	{
		printf("  tk %d: Type = %d, Value = '%s'\n", 
			i, 
			tks[i]->type, 
			tks[i]->value ? tks[i]->value : "(null)");
		i++;
	}
	printf("Total tks: %d\n", i);
}

