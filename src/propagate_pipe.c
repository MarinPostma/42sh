/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   propagate_pipe.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gchainet <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/10 08:57:52 by gchainet          #+#    #+#             */
/*   Updated: 2019/01/10 09:05:44 by gchainet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>

#include "shell.h"
#include "ast.h"

int	propagate_pipe_left(t_ast *pipe, t_ast *instr)
{
	instr->pipes_in[PIPE_PARENT][STDIN_FILENO] =\
		pipe->pipes_in[PIPE_PARENT][STDIN_FILENO];
	instr->pipes_in[PIPE_PARENT][STDOUT_FILENO] =\
		pipe->pipes_in[PIPE_PARENT][STDOUT_FILENO];
	instr->pipes_out[PIPE_PARENT][STDIN_FILENO] =\
		pipe->pipes_in[PIPE_NODE][STDIN_FILENO];
	instr->pipes_out[PIPE_PARENT][STDOUT_FILENO] =\
		pipe->pipes_in[PIPE_NODE][STDOUT_FILENO];
	if (instr->left)
		propagate_pipe_left(pipe, instr->left);
	if (instr->right)
		propagate_pipe_left(pipe, instr->right);
	return (0);
}
