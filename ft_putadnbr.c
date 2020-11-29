/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putadnbr_fd.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysakuma <ysakuma@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/13 22:52:46 by ysakuma           #+#    #+#             */
/*   Updated: 2020/11/25 10:34:26 by ysakuma          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

void	ft_putadnbr(uintptr_t nbr, t_flag **flag)
{
	if (nbr >= 16)
	{
		ft_putadnbr(nbr / 16, flag);
		ft_putadnbr(nbr % 16, flag);
	}
	else
		ft_putchar("0123456789abcdef"[nbr], flag);
}
