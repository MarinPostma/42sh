/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gchainet <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/12/12 09:48:47 by gchainet          #+#    #+#             */
/*   Updated: 2019/01/21 21:27:50 by gchainet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

#include "shell.h"
#include "libft.h"
#include "fill_line.h"

static int	increment_shlvl(t_shell *shell)
{
	char	*new_value;
	char	*shlvl;
	int		old_value;

	if ((shlvl = get_env_value(shell, "SHLVL")))
		old_value = ft_atoi(shlvl);
	else
		old_value = 0;
	if (!(new_value = ft_ltoa_base(old_value + 1, 10)))
		return (1);
	set_env_var(shell, "SHLVL", new_value);
	free(new_value);
	return (0);
}

void		free_shell(t_shell *shell)
{
	int	i;

	i = 0;
	if (shell->env)
	{
		while (shell->env[i])
			free(shell->env[i++]);
		free(shell->env);
	}
	free_line(&shell->line);
	i = 0;
	while (i < HASH_TABLE_SIZE)
	{
		if (shell->hash_table[i])
		{
			free(shell->hash_table[i]->path);
			free(shell->hash_table[i]->bin);
			free(shell->hash_table[i]);
		}
		++i;
	}
	free(shell->hash_table);
	reset_terminal_mode();
}

int			init_shell(t_shell *shell, char **environ)
{
	if (init_lexer(&shell->lexer) || init_parser(&shell->parser))
		return (1);
	shell->hash_table = malloc(sizeof(*shell->hash_table) * HASH_TABLE_SIZE);
	if (!shell->hash_table)
		return (1);
	ft_bzero(shell->hash_table, sizeof(*shell->hash_table) * HASH_TABLE_SIZE);
	ft_bzero(&shell->line, sizeof(shell->line));
	if (!(shell->env = copy_env(environ)))
	{
		ft_dprintf(2, "%s: %s\n", EXEC_NAME, MEMORY_ERROR_MSG);
		return (1);
	}
	if (increment_shlvl(shell))
	{
		ft_dprintf(2, "%s: %s\n", EXEC_NAME, MEMORY_ERROR_MSG);
		remove_env(shell);
		return (1);
	}
	shell->his_pos = -1;
	raw_terminal_mode();
	shell->history = NULL;
	disable_signal();
	load_history(shell);
	return (0);
}
