/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   while.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gchainet <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/12/31 17:57:58 by gchainet          #+#    #+#             */
/*   Updated: 2019/01/12 17:41:57 by gchainet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>

#include "shell.h"
#include "parser.h"
#include "ast.h"

int		exec_while(t_shell *shell, t_ast *ast)
{
	redir_save(ast->redir_list, ast);
	((t_ast *)ast->data)->exec(shell, ast->data);
	wait_loop(ast->data);
	while (((t_ast *)ast->data)->ret == 0)
	{
		ast->left->exec(shell, ast->left);
		wait_loop(ast->left);
		((t_ast *)ast->data)->exec(shell, ast->data);
		wait_loop(ast->data);
	}
	reset_redirs(ast);
	return (ast->left->ret);
}

void	free_while(t_ast *ast)
{
	if (ast->left)
		ast->left->del(ast->left);
	free_ast(ast);
}