/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sig_msg.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fstadelw <fstadelw@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/12/18 04:20:06 by fstadelw          #+#    #+#             */
/*   Updated: 2019/03/24 07:18:51 by fstadelw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SIG_MSG_H
# define SIG_MSG_H

# include <signal.h>

/*
** used to match an Error message to it's value
*/
typedef struct	s_sig_msg
{
	pid_t		sig;
	const char	*msg;
	uint8_t		rtn;
}				t_sig_msg;

#endif
