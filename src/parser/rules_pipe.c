/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rules_pipe.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gchainet <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/12/19 20:14:32 by gchainet          #+#    #+#             */
/*   Updated: 2019/01/03 11:47:09 by gchainet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ast.h"
#include "21sh.h"
#include "parser.h"

int	rule_pipe(t_parser *parser, t_ast_token *list)
{
	t_ast		*pipe_node;
	t_ast_token	*tmp;

	(void)parser;
	pipe_node = alloc_ast(NULL, TT_PIPE, &exec_pipeline, &free_pipeline);
	if (!pipe_node)
		return (1);
	pipe_node->left = list->data;
	pipe_node->right = list->next->next->data;
	tmp = list->next->next->next;
	free(list->next->next);
	free(list->next->data);
	free(list->next);
	list->next = tmp;
	list->data = pipe_node;
	return (0);
}
