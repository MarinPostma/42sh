/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rules.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gchainet <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/12/15 08:20:26 by gchainet          #+#    #+#             */
/*   Updated: 2019/02/11 22:32:35 by gchainet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>

#include "ast.h"
#include "shell.h"
#include "libft.h"

int	rule_first_word(t_parser *parser, t_ast_token *list)
{
	int	type;

	type = keyword_type(list->data);
	if (type != TT_WORD)
		list->type = type;
	else
		return (rule_push_cmd(parser, list));
	return (0);
}

int	rule_push_cmd(t_parser *parser, t_ast_token *list)
{
	t_ast	*node;

	(void)list;
	node = alloc_ast(NULL, TT_CMD, &exec_cmd, &free_cmd);
	if (!node || pss_push(parser, PS_CMD))
		return (1);
	node->data = alloc_command();
	if (!node->data)
	{
		free(node);
		return (1);
	}
	parser->pss->ret = node;
	return (0);
}

int	rule_add_to_cmd(t_parser *parser, t_ast_token *list)
{
	if (add_to_command(parser->pss->ret->data, list->data))
		return (1);
	free(pop_ast_token(&parser->input_queue));
	return (0);
}

int	rule_create_end(t_parser *parser, t_ast_token *list)
{
	t_ast		*node_end;

	(void)parser;
	if (!(node_end = alloc_ast(NULL, TT_END, exec_end, free_end)))
		return (1);
	free(list->data);
	list->data = node_end;
	list->type = TT_OP;
	return (0);
}

int	rule_create_end_second(t_parser *parser, t_ast_token *list)
{
	t_ast	*node_end;

	(void)parser;
	node_end = alloc_ast(NULL, TT_END, exec_end, free_end);
	if (!node_end)
		return (1);
	free(list->next->data);
	list->next->data = node_end;
	list->next->type = TT_OP;
	return (0);
}
