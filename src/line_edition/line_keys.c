/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   line_keys.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cvignal <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/12/26 19:02:11 by cvignal           #+#    #+#             */
/*   Updated: 2019/03/12 17:46:01 by cvignal          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <term.h>
#include <curses.h>
#include <termios.h>
#include <sys/ioctl.h>

#include "shell.h"
#include "libft.h"
#include "fill_line.h"

static void	down_one_line(t_line line, int width, int len_prompt, int flag)
{
	t_curs	cursor;
	int		i;
	int		len_last_line;

	get_cursor_pos(&cursor);
	tputs(tgoto(tgetstr("cm", NULL), cursor.col - 1, cursor.line)
			, 0, ft_printchar);
	len_last_line = (line.len + len_prompt) % width;
	if (flag)
		i = cursor.col - len_last_line - 1;
	else
		i = 0;
	while (i > 0)
	{
		t_puts("le");
		i--;
	}
}

static void	back_to_startline(int len_prompt)
{
	t_curs		cursor;
	int			i;

	get_cursor_pos(&cursor);
	i = cursor.col - (len_prompt + 1);
	while (i < 0)
	{
		t_puts("nd");
		i++;
	}
}

int			nb_multi_lines(size_t len, int len_prompt)
{
	int				length;
	int				width;
	struct winsize	win;
	int				nb;

	length = (int)len;
	ioctl(0, TIOCGWINSZ, &win);
	width = win.ws_col;
	if (length + len_prompt >= width)
		length -= width - len_prompt;
	else
		return (0);
	nb = 1;
	while ((length -= width) > 0)
		nb++;
	return (nb);
}

int			ft_lineup(t_shell *shell)
{
	int				cursor_nb;
	int				width;
	struct winsize	win;

	ioctl(0, TIOCGWINSZ, &win);
	width = win.ws_col;
	cursor_nb = nb_multi_lines(shell->line.cursor, shell->prompt_len);
	if (cursor_nb > 0)
	{
		t_puts("up");
		if (cursor_nb == 1)
			back_to_startline(shell->prompt_len);
		if (shell->line.cursor < (size_t)width)
			shell->line.cursor = 0;
		else
			shell->line.cursor -= width;
	}
	return (0);
}

int			ft_linedown(t_shell *shell)
{
	int				line_nb;
	int				cursor_nb;
	int				width;
	struct winsize	win;

	ioctl(0, TIOCGWINSZ, &win);
	width = win.ws_col;
	line_nb = nb_multi_lines(shell->line.len, shell->prompt_len);
	cursor_nb = nb_multi_lines(shell->line.cursor, shell->prompt_len);
	if (cursor_nb < line_nb)
	{
		down_one_line(shell->line, width, shell->prompt_len
				, (cursor_nb == line_nb - 1));
		shell->line.cursor += width;
		if (shell->line.cursor > shell->line.len)
			shell->line.cursor = shell->line.len;
	}
	return (0);
}
