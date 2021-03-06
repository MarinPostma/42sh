/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tools_insertion.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cvignal <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/12 11:44:35 by cvignal           #+#    #+#             */
/*   Updated: 2019/02/18 14:11:45 by cvignal          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <term.h>
#include <curses.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <fcntl.h>

#include "libft.h"
#include "shell.h"
#include "fill_line.h"

int			pos_cursor_col(t_shell *shell, int width, int len)
{
	int	ret;
	int	length;

	if (!nb_multi_lines(shell->line.cursor - len, shell->prompt_len))
		ret = shell->prompt_len + shell->line.cursor;
	else
	{
		length = (int)shell->line.cursor;
		length -= width - shell->prompt_len;
		ret = length % width;
	}
	if (ret <= 0)
		ret = width;
	return (ret);
}

static void	print_multi_lines(t_shell *shell, char buf)
{
	int				curs_col;
	struct winsize	win;

	ioctl(0, TIOCGWINSZ, &win);
	t_puts("ce");
	curs_col = pos_cursor_col(shell, win.ws_col, 1);
	ft_dprintf(shell->fd_op, "%c", buf);
	t_puts("sc");
	ft_dprintf(shell->fd_op, "%s", shell->line.data + shell->line.cursor);
	t_puts("rc");
	if (curs_col == win.ws_col)
		t_puts("do");
}

void		print_line(t_shell *shell, char buf)
{
	int				curs_col;
	struct winsize	win;

	ioctl(0, TIOCGWINSZ, &win);
	if (!nb_multi_lines(shell->line.len, shell->prompt_len)
			|| shell->line.len == shell->line.cursor)
	{
		curs_col = pos_cursor_col(shell, win.ws_col, 1);
		t_puts("im");
		ft_dprintf(shell->fd_op, "%c", buf);
		t_puts("ei");
		if (curs_col == win.ws_col)
			t_puts("do");
	}
	else
		print_multi_lines(shell, buf);
}
