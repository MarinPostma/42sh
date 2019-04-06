/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gchainet <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/12/12 07:55:15 by gchainet          #+#    #+#             */
/*   Updated: 2019/04/06 03:44:02 by gchainet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>

#include "shell.h"
#include "libft.h"
#include "ast.h"
#include "libft.h"
#include "arithmetic.h"

static void		debug(t_token *token)
{
	while (token)
	{
		ft_printf("%d %s\n", token->type, token->data);
		token = token->next;
	}
}

int				clean_exit_lexer(t_lexer *lexer, t_token **list
		, t_token **current, const char *msg)
{
	t_token	*tmp;

	if (msg)
		ft_dprintf(2, "%s: %s\n", EXEC_NAME, msg);
	while (*list)
	{
		tmp = *list;
		*list = (*list)->next;
		if (tmp != *current)
		{
			free(tmp->data);
			free(tmp);
		}
	}
	if (current && *current)
	{
		free((*current)->data);
		free(*current);
		*current = NULL;
	}
	while (lexer->lss->state != LSTATE_NONE)
		lss_pop(lexer);
	return (1);
}

static int		handle_ret(t_lexer *lexer, int ret, t_token **current,
		t_token **output)
{
	if (ret & (1 << LEXER_RET_ERROR))
		return (clean_exit_lexer(lexer, output, current, SYNTAX_ERROR_MSG));
	if (ret & (1 << LEXER_RET_CREATE))
	{
		if (!(*current = alloc_token()))
			return (clean_exit_lexer(lexer, output, current, MEMORY_ERROR_MSG));
	}
	if (ret & (1 << LEXER_RET_CUT))
	{
		(*current)->type = get_token_type(lexer, *current);
		if (((*current)->type == TT_ARI_BEGIN))
		{
			if (lss_push(lexer, LSTATE_ARI_NONE))
				return (clean_exit_lexer(lexer, output, current,
							MEMORY_ERROR_MSG));
		}
		add_to_token_list(output, *current);
		*current = NULL;
	}
	if (ret & (1 << LEXER_RET_MORE_INPUT))
		return (1);
	return (0);
}

static t_token	*get_return(t_token **output, int *pos)
{
	t_token		*over;
	t_token		*ret;

	if (!(over = alloc_token()))
		return (NULL);
	over->type = TT_OVER;
	add_to_token_list(output, over);
	ret = *output;
	set_unary_operator(ret);
	*output = NULL;
	*pos = 0;
	return (ret);
}

t_token			*lex(t_shell *shell)
{
	int				pos;
	int				ret;

	ret = 0;
	pos = 0;
	while (!(ret & (1 << LEXER_RET_OVER)))
	{
		if (shell->line.data[pos] >= 0)
		{
			ret = shell->lexer.lexer_actions[shell->lexer.lss->state]
				[(int)shell->line.data[pos]](shell, shell->current,
						shell->line.data[pos]);
			if (handle_ret(&shell->lexer, ret, &shell->current, &shell->output))
				return (NULL);
			pos += !!(ret & (1 << LEXER_RET_CONT));
		}
		else
		{
			clean_exit_lexer(&shell->lexer, &shell->output, &shell->current
					, SYNTAX_ERROR_MSG);
			return (NULL);
		}
	}
	debug(shell->output);
	return (get_return(&shell->output, &pos));
}
