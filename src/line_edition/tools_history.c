/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tools_history.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cvignal <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/21 16:47:09 by cvignal           #+#    #+#             */
/*   Updated: 2019/02/08 15:27:49 by cvignal          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>

#include "shell.h"
#include "fill_line.h"
#include "libft.h"

int			load_history(t_shell *shell)
{
	char	*line;
	t_list	*new;

	shell->history = NULL;
	shell->output = NULL;
	shell->current = NULL;
	if ((shell->fd_hf = open(".shperso_history", O_RDWR | O_APPEND
					| O_CREAT, 0644)) == -1)
		return (0);
	while (get_next_line(shell->fd_hf, &line) == 1)
	{
		if (!(new = ft_lstnew(line, ft_strlen(line) + 1)))
			return (0);
		ft_lstadd(&shell->history, new);
		free(line);
	}
	if (!check_validity(shell))
	{
		if ((shell->fd_op = open(ttyname(0), O_WRONLY)) < 0)
			return (0);
		g_fd_output = shell->fd_op;
	}
	return (1);
}

static int	add_complete_command(char *str, t_shell *shell, char **multi_line)
{
	t_list	*new;

	if (*multi_line)
	{
		if (!(*multi_line = ft_strjoin_free(*multi_line, str, 1)))
			return (1);
		free(shell->history->content);
		if (!(shell->history->content = ft_strdup(*multi_line)))
			return (1);
		shell->history->content_size = ft_strlen(*multi_line) + 1;
		ft_dprintf(shell->fd_hf, "%s\n", *multi_line);
		ft_strdel(multi_line);
		return (0);
	}
	if (!(new = ft_lstnew(str, ft_strlen(str) + 1)))
		return (1);
	ft_lstadd(&shell->history, new);
	ft_dprintf(shell->fd_hf, "%s\n", str);
	return (0);
}

int			add_to_history(char *str, t_shell *shell, int flag)
{
	t_list		*new;
	static char	*multi_line = NULL;

	if (flag && multi_line)
	{
		if (!(multi_line = ft_strjoin_free(multi_line, str, 1)))
			return (1);
		if (!(multi_line = ft_strjoin_free(multi_line, "\n", 1)))
			return (1);
		free(shell->history->content);
		if (!(shell->history->content = ft_strdup(multi_line)))
			return (1);
		shell->history->content_size = ft_strlen(multi_line) + 1;
	}
	else if (flag && !multi_line)
	{
		if (!(multi_line = ft_strjoin_free(str, "\n", 0)))
			return (1);
		if (!(new = ft_lstnew(str, ft_strlen(str) + 1)))
			return (1);
		ft_lstadd(&shell->history, new);
	}
	else if (!flag && ft_strlen(str))
		return (add_complete_command(str, shell, &multi_line));
	return (0);
}
