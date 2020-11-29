/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putxnbr.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysakuma <ysakuma@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/13 22:52:46 by ysakuma           #+#    #+#             */
/*   Updated: 2020/11/24 22:11:16 by ysakuma          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

void	ft_putxnbr(unsigned int nbr, t_flag **flag)
{
	if (nbr >= 16)
	{
		ft_putxnbr(nbr / 16, flag);
		ft_putxnbr(nbr % 16, flag);
	}
	else
		ft_putchar("0123456789abcdef"[nbr], flag);
}
