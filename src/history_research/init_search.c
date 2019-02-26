/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_search.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cvignal <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/19 16:23:43 by cvignal           #+#    #+#             */
/*   Updated: 2019/02/20 16:59:45 by cvignal          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <term.h>
#include <curses.h>
#include <stdlib.h>
#include <unistd.h>

#include "shell.h"
#include "fill_line.h"

t_key		g_hs_keys[] =\
{
	{LEFT_ARROW, &hs_leftkey},
	{RIGHT_ARROW, &hs_rightkey},
	{DOWN_ARROW, &hs_downkey},
	{UP_ARROW, &hs_upkey},
	{HOME_KEY, &hs_homekey},
	{END_KEY, &hs_endkey},
	{MAJ_LEFT, &hs_prevword},
	{MAJ_RIGHT, &hs_nextword},
	{MAJ_UP, &hs_lineup},
	{MAJ_DOWN, &hs_linedown},
	{CTRL_D, &hs_ctrld},
	{CTRL_C, &ft_ctrlc},
	{CTRL_R, &hs_ctrlr},
	{CTRL_A, &hs_homekey},
	{CTRL_HOME, &hs_homekey},
	{CTRL_END, &hs_endkey},
	{ALT_HOME, &hs_homekey},
	{ALT_END, &hs_endkey}
};

int			hs_specialkeys(char *buf)
{
	size_t	i;
	size_t	len;

	i = 0;
	while (i < sizeof(g_hs_keys) / sizeof(*g_hs_keys))
	{
		len = ft_strlen(g_hs_keys[i].value);
		if (ft_strnequ(g_hs_keys[i].value, buf, len))
			return (1);
		i++;
	}
	return (0);
}

int			hs_keyapply(char *buf, t_shell *shell)
{
	size_t	i;
	size_t	len;

	i = 0;
	while (i < sizeof(g_hs_keys) / sizeof(*g_hs_keys))
	{
		len = ft_strlen(g_hs_keys[i].value);
		if (ft_strnequ(g_hs_keys[i].value, buf, len))
			return (g_hs_keys[i].f(shell));
		i++;
	}
	return (0);
}

static void	hs_backspace(t_shell *shell)
{
	size_t	nb;
	size_t	len;

	nb = 5 + shell->line.curs_search;
	len = ft_strlen(shell->line.search);
	if (!len)
		return ;
	shell->line.search[len - 1] = 0;
	if (shell->line.len_search < len)
		return ;
	while (--nb)
		t_puts("le");
	t_puts("dc");
	while (++nb != 4 + shell->line.curs_search)
		t_puts("nd");
	shell->line.len_search--;
	shell->his_pos = -1;
}

int			hs_addchar(char *buf, t_shell *shell)
{
	int	i;
	int	nb;

	i = 0;
	nb = 4;
	while (buf[i])
	{
		if (buf[i] == 10 || buf[i] == 13)
			return (1);
		else if (buf[i] >= 32 && buf[i] <= 126)
		{
			shell->line.search[ft_strlen(shell->line.search)] = buf[i];
			if (hs_search(shell, 0))
				return (0);
			display_char_in_research(shell, buf[i]);
		}
		else if (buf[i] == 127)
			hs_backspace(shell);
		i++;
	}
	return (0);
}

int			ft_ctrlr(t_shell *shell)
{
	int		ret;
	char	buf[9];
	int		res;

	t_puts("cr");
	t_puts("dl");
	ft_dprintf(shell->fd_op, "(reverse-i-search)`': ");
	ft_bzero(shell->line.search, SEARCH_MAX);
	ft_strdel(&shell->pbpaste);
	res = 0;
	while (!res && (ret = read(0, buf, 8)))
	{
		buf[ret] = 0;
		if (hs_specialkeys(buf))
		{
			if (hs_keyapply(buf, shell))
				break ;
		}
		else
			res = hs_addchar(buf, shell);
	}
	fill_line_hs(shell, buf);
	return (res);
}