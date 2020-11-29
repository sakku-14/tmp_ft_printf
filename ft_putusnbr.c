/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putusnbr.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysakuma <ysakuma@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/13 22:52:46 by ysakuma           #+#    #+#             */
/*   Updated: 2020/11/24 22:05:29 by ysakuma          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

void	ft_putusnbr(unsigned int nbr, t_flag **flag)
{
	if (nbr >= 10)
	{
		ft_putusnbr(nbr / 10, flag);
		ft_putusnbr(nbr % 10, flag);
	}
	else
		ft_putchar(nbr + '0', flag);
}
